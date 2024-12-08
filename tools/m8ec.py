import os
import subprocess
import argparse
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), "..",
                "extern", "W25Q64_STM32H750VB-DevEBox", "tools", "comm"))
from comm import Comm


def sys_cmd(cmd):
    print(' '.join(cmd))
    try:
        if subprocess.call(cmd) != 0:
            raise Exception(f"Command failed: {' '.join(cmd)}")
    except KeyboardInterrupt:
        pass


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--clean", help="clean build",
                        action="store_true")
    parser.add_argument(
        "-b", "--build", help="build project", action="store_true")
    parser.add_argument("-t", "--build_target",
                        help="build target (Release or Debug)", default="Release")
    parser.add_argument(
        "--build_dir", help="build directory", default=".build")
    parser.add_argument(
        "-f", "--flash", help="flash (built) firmware to device", action="store_true")
    parser.add_argument("-r", "--reset", help="reset board",
                        action="store_true")
    parser.add_argument("-p", "--platform", help="platform",
                        choices=["STM32H750", "STM32F411"], default="STM32H750")
    parser.add_argument("--serial", help="enable serial", action="store_true")
    parser.add_argument(
        "--serial_dev", help="serial device for flashing and potentially for interfacing application", default="/dev/ttyACM0")
    parser.add_argument("--serial_log", help="serial log path",
                        default="serial.log")
    parser.add_argument(
        "--sysview", help="enable sysview", action="store_true")
    args = parser.parse_args()

    if args.sysview and args.platform != "STM32H750":
        raise Exception("Sysview is only supported on STM32H750 platform. Use `-p STM32H750`")
    if args.sysview and args.build_target != "Debug":
        raise Exception("Sysview is only supported on Debug build target. Use `-t Debug`")

    if args.clean:
        print("Cleaning project")
        sys_cmd(["rm", "-rf", args.build_dir])

    if args.build:
        print("Building project")
        sys_cmd(
            [
                "cmake",
                "-S",
                ".",
                "-B",
                args.build_dir,
                "-DPLATFORM={}".format(args.platform),
                "-G",
                "Ninja",
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                "-DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake",
                "-DCMAKE_BUILD_TYPE={}".format(args.build_target),
                "-DSEGGER_SYSVIEW_ENABLED={}".format(
                    "ON" if args.sysview else "OFF"),
            ]
        )
        sys_cmd(["cmake", "--build", args.build_dir, "--", "-j4"])

    # flashing requires reset to enter bootloader
    if args.reset or args.flash:
        print("Resetting board")
        sys_cmd(["openocd", "-f", "openocd.jlink.cfg",
                "-c", "init;reset;shutdown"])

    if args.flash:
        comm = Comm(args.serial_dev)
        comm.await_bootloader(10.0)
        comm.transfer_file(
            f".build/platform/{args.platform}/{args.platform}.bin", "boot/app.bin")
        comm.release_bootloader()

    if args.serial:
        sys_cmd(["minicom", "--baudrate", "921600", "-D",
                args.serial_dev, "-C", args.serial_log])


if __name__ == "__main__":
    main()
