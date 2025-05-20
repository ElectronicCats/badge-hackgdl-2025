# Badge HackGDL 2025

<p align="center">
    <a href="https://github.com/ElectronicCats/badge-hackgdl-2025/wiki">
        <img src=imagen del badge height=500>
    </a>
</p>

<p align=center>
    <a href="https://github.com/ElectronicCats/Minino/wiki">
        <img src="https://github.com/ElectronicCats/flipper-shields/assets/44976441/6aa7f319-3256-442e-a00d-33c8126833ec" width="200" height="104" />
    </a>
</p>

Welcome to the official repository of the Hack GDL 2025 event badge, developed by Electronic Cats. This portable device is designed to be interactive, educational, and hackable. It is based on the [Minino](https://github.com/ElectronicCats/Minino) platform, which uses ESP-IDF as the development environment and offers a modular system for creating BLE and Wi-Fi applications.

## Features:
- ESP32-S3
- Bluetooth Low Energy (BLE) + Wi-Fi
- USB-C for power and programming
- 4 x physical buttons for interaction
- 3 neopixel
- Integrated OLED Screen
- 3x AAA Battery Holder
- 1 x Buzzer
- Shitty Addon Connector
- Case
- Based on [Minino](https://github.com/ElectronicCats/Minino), with a modular architecture for multiple apps

## Included Applications:
These applications are ready-to-use and serve as a base for creating your own ideas during the event.
- Bluetooth (BLE) Applications
- Wi-Fi Applications
- Digital Tamagotchi

## Flashing Firmware:
Your badge should come with the latest firmware. If an update is required, this section of the wiki will explain how to flash your badge with the latest firmware. 

### Bootloader mode

To be able to flash your board, you will need to understand how to put it into bootloader mode. You just need to follow the sequence below:

* Press and hold the **BOOT** button
* Press and release the **RST** button
* Release the **BOOT** button


### ESP Tool (Web) update

1. Download the latest firmware from the [releases](https://github.com/ElectronicCats/badge-hackgdl-2025/releases) section.

2. Extract the downloaded file.

3. Open your web browser and go to [ESP Tool](https://espressif.github.io/esptool-js/).

4. Connect your badge to your computer using a USB cable and enter [bootloader mode](https://github.com/ElectronicCats/badge-hackgdl-2025/edit/main/README.md#bootloader-mode). 

5. Once on the page, in the baud rate, select 460800 and click on the “Connect” button.

6. You will see a pop-up window asking you to select the port where your badge is detected. Choose the one labeled “USB JTAG/serial debug unit.”

7. Once your badge is connected, the website will display the “Flash Address” field. Enter 0x0 in that field.

8. Click on “Select File” and look for the files in the unzipped folder. You must match the file and address according to the following table:

| Flash Address | File                 |
|---------------|----------------------|
| 0x0           | HackGDL_2025.bin |


9. Once you have all the files, click on “Program.” Wait until the process ends, or you read the message “Leaving...” in the output console.

<p align=center>
<img width=100% src="https://github.com/user-attachments/assets/21b3896a-ed20-4d3c-a8b2-639fc9ce847f"/>
</p>

10. When the program is finished, click on the “Disconnect” button.
11. Reset your HackGDL 2025 badge.

### Using the console

>[!IMPORTANT]
> [esptool.py](https://pypi.org/project/esptool/) is required.

To update your badge’s firmware, follow these steps:

1. Download the latest firmware from the [releases](https://github.com/ElectronicCats/badge-hackgdl-2025/releases).

3. Extract the downloaded file.

3. Open a terminal and navigate to the extracted folder. Make sure you see the HackGDL_2025.bin file.

4. Connect your badge to your computer using a USB cable and enter [bootloader mode](https://github.com/ElectronicCats/badge-hackgdl-2025/edit/main/README.md#bootloader-mode). Then check the port it is connected to.
    - **Windows**: Check it in the Device Manager under the Ports section.
    - **Linux**: check it with the `ls /dev/ttyUSB*` or `ls /dev/ttyACM*` command.
    - **macOS**: Check it with the `ls /dev/cu.usb*` command.

5. Run the following command to flash the firmware:

```python
python -m esptool --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 HackGDL_2025.bin
```

## Development Reference:

To compile, modify, or develop new applications for this badge, follow the official [Minino firmware development guide](https://github.com/ElectronicCats/Minino/tree/main/firmware).

> [!NOTE]  
> Although the Minino platform was designed for ESP32-C6, the HackGDL 2025 badge uses an ESP32-S3 as the main microcontroller. Make sure to configure the correct chip target in your development environment.

It includes:
ESP-IDF installation (v5.1.2)
Environment setup
Firmware compilation
Modular app structure
Flash and monitor commands

## FAQs
**Does the badge work out of the box?**

Yes. It comes with functional BLE and WiFi-based apps pre-installed.

**Can I develop my own app?**

Absolutely! Use Minino’s modular system and examples as a starting point.

**Where can I get help?**

Check the Minino wiki or open an issue [here](https://github.com/ElectronicCats/badge-hackgdl-2025/issues).


## Gratitude
Do you want to say thank you? Tag these companies on social networks and show them your badge, thanks to them, it is possible

- [ElectronicCats](https://electroniccats.com/)
- [HackGDL](https://hackgdl.net/)
- [Project TamaLib: A hardware-agnostic first-gen Tamagotchi emulation library](https://github.com/jcrona/tamalib)

## License

<a href="https://github.com/ElectronicCats">
    <img src="https://github.com/ElectronicCats/AjoloteBoard/raw/master/OpenSourceLicense.png" height="200" />
</a>

Electronic Cats invests time and resources providing this open source design, please support Electronic Cats and open-source hardware by purchasing products from Electronic Cats!

Designed by Electronic Cats.

Firmware released under an GNU AGPL v3.0 license. See the LICENSE file for more information.

Hardware released under an CERN Open Hardware License v1.2. See the LICENSE_HARDWARE file for more information.

Electronic Cats is a registered trademark, please do not use if you sell these PCBs.

Nov 29 2022
