import sys
import argparse
import subprocess
import re
import time
import glob
import os

def sys_cmd(cmd):
    print(' '.join(cmd))
    try:
        return subprocess.call(cmd)
    except KeyboardInterrupt:
        pass


def sys_cmd_wait_pattern(cmd, success_pattern, failure_pattern, timeout):
    print(' '.join(cmd))
    start_time = time.time()
    process = subprocess.Popen(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        output = process.stdout.readline().decode()
        print(output, end='')
        if re.search(failure_pattern, output):
            process.terminate()
            raise Exception("Failure pattern reached")
        if time.time() - start_time > timeout:
            process.terminate()
            raise Exception("Timeout reached")
        if re.search(success_pattern, output):
            process.terminate()
            break
    return process.wait()


def usbipd_get_device_details(device_name):
    usbipd_list_stdout = subprocess.check_output([args.usbipd, "list"])
    usb_dev = re.search(USBIPD_LIST_CONNECTED_REGEX +
                        device_name, usbipd_list_stdout.decode("utf-8"))
    print(
        f'"{device_name}" USB device detected:'
        f'bus {usb_dev["bus"]}, '
        f'device {usb_dev["device"]}, '
        f'VID {usb_dev["VID"]}, '
        f'PID {usb_dev["PID"]}')
    return usb_dev


def usbipd_attach_device(device_name):
    USB_DEVICE = usbipd_get_device_details(device_name)
    try:
        sys_cmd_wait_pattern(
            [
                args.usbipd,
                "attach",
                "-w",
                "-i",
                f"{USB_DEVICE['VID']}:{USB_DEVICE['PID']}"
            ],
            "WSL Attached|already attached to a client",
            "Device is not shared",
            5)
    except Exception:
        print(
            f"Open powershell as admin and run: 'usbipd bind -b {USB_DEVICE['bus']}-{USB_DEVICE['device']}'")
        sys.exit(1)
    # sys_cmd(
    #     [
    #         "chmod",
    #         "666",
    #         f"/dev/bus/usb/{int(USB_DEVICE['bus']):03d}/{int(USB_DEVICE['device']):03d}"
    #     ]
    # )


parser = argparse.ArgumentParser()
parser.add_argument("-c", "--clean", help="clean build", action="store_true")
parser.add_argument("-b", "--build", help="build project", action="store_true")
parser.add_argument("-d", "--debug", help="enable debug", action="store_true")
parser.add_argument("-f", "--flash", help="flash project", action="store_true")
parser.add_argument("-r", "--reset", help="reset board", action="store_true")
parser.add_argument("-p", "--platform", help="platform", default="STM32H750")
parser.add_argument("--wsl", help="enable WSL USB access", action="store_true")
parser.add_argument("--usbipd", help="path to usbipd.exe",
                    default=r"/mnt/c/Program Files/usbipd-win/usbipd.exe")
parser.add_argument(
    "--stlink", help="name of the STLink USB device", default="STM32 STLink")
parser.add_argument("--serial", help="enable serial", action="store_true")
parser.add_argument(
    "--serial-name", help="name of the serial USB device",   default="CP2102")
parser.add_argument(
    "--systemview", help="enable systemview", action="store_true")
args = parser.parse_args()

BUILD_DIR = ".build"

USBIPD_LIST_CONNECTED_REGEX = r"(?P<bus>\d+)-(?P<device>\d+).*(?P<VID>[0-9a-fA-F]{4}):(?P<PID>[0-9a-fA-F]{4})\s+"

if args.serial:
    usbipd_attach_device(args.serial_name)
    try:
        sys_cmd(["minicom", "--version"])
    except FileNotFoundError:
        print("Error: minicom not found")
        sys.exit(1)
    serial_dev = glob.glob(f"/dev/serial/by-id/usb-*{args.serial_name}*")[0]
    if not serial_dev:
        print(f"Error: serial device {args.serial_name} not found")
        sys.exit(1)
    if not os.path.exists(".tmp"):
        os.makedirs(".tmp")
    sys_cmd(["minicom", "--baudrate", "921600",
            "-D", serial_dev, "-C", ".tmp/serial.log"])
    sys.exit(0)

if args.clean:
    print("Cleaning project")
    error = sys_cmd(["rm", "-rf", BUILD_DIR])
    if error != 0:
        print("Error: clean failed")
        sys.exit(1)

if args.build:
    print("Building project")
    error = sys_cmd(
        [
            "cmake",
            "-S",
            ".",
            "-B",
            BUILD_DIR,
            "-DPLATFORM={}".format(args.platform),
            "-G",
            "Unix Makefiles",
            "-DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake",
            "-DCMAKE_BUILD_TYPE={}".format(
                "Debug" if args.debug else "Release"),
            "-DSEGGER_SYSVIEW={}".format(
                "ON" if args.systemview else "OFF"),
        ]
    )
    if error != 0:
        print("Error: cmake failed")
        sys.exit(1)
    error = sys_cmd(["cmake", "--build", BUILD_DIR, "--", "-j4"])
    if error != 0:
        print("Error: build failed")
        sys.exit(1)

if args.flash:
    if args.wsl:
        try:
            sys_cmd([args.usbipd, "--version"])
        except FileNotFoundError:
            print("Error: usbipd.exe not found")
            sys.exit(1)
        usbipd_attach_device(args.stlink)
    if args.build and error != 0:
        print("Error: build failed skipping flash")
        sys.exit(1)
    sys_cmd(
        [
            "st-flash",
            "--freq=4000",
            "--format",
            "ihex",
            "write",
            f".build/platform/{args.platform}/{args.platform}.hex"
        ]
    )

if args.reset or args.flash:
    print("Resetting board")
    sys_cmd(["st-flash", "reset"])
    # alternative: `openocd -f "interface/stlink-v2.cfg" -f "target/stm32f4x.cfg" -c "init;reset;exit"`

if args.debug and args.flash:
    print("Starting debug session")
    try:
        sys_cmd(["killall", "openocd"])
        sys_cmd(["openocd"])
    except KeyboardInterrupt:
        pass
