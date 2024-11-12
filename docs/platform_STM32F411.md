# Platform STM32F411 (Black Pill)

This platform is abandoned at the moment. This section is intended to preserve
knowledge for this platform in case it is picked up again.

## Flashing the Built Firmware

1. [WSL ONLY] [Install the USBIPD-Win
   project](https://learn.microsoft.com/en-us/windows/wsl/connect-usb#install-the-usbipd-win-project).
2. Install ST-Link tools.

    ```bash
    sudo apt-get install -y stlink-tools
    ```

3. Plug in ST-LINK/V2 SWD interface to the target MCU.
4. Plug in ST-LINK/V2 USB interface to your PC.
3. Flash using the project tool.

    ```bash
    sudo python3 tools/m8ec.py -f -p <PLATFORM>
    ```

## OpenOCD + VS Code + Cortex-Debug

1. Install openOCD.

    ```bash
    sudo apt-get install -y openocd
    ```

2. Open the project in VS Code.
3. Install the `marus25.cortex-debug` extension in VS Code.
4. Build debug version of the firmware and flash it to the target MCU.

    ```bash
    python3 tools/m8ec.py -t Debug -bf
    ```

    > When flashing the debug version, the m8ec.py tool will finish by starting
    > openOCD. You can stop it with `Ctrl+C`.

5. In VS Code, press `F5` or open the Debug tab and click the green arrow to
   start debugging.
