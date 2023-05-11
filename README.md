# GEVCU7
======

Generalized Electric Vehicle Control Unit

Creator's website can be found at : http://www.evtv.me

A project to create a fairly Arduino compatible ECU firmware
to interface with various electric vehicle hardware over CANbus
(and possibly other comm channels)

The project now builds in the Arduino IDE. So, use it to compile, send the firmware to the GEVCU hardware, and monitor serial. It all works very nicely.

## Getting Started
You will need the following to have any hope of compiling and running the firmware:

NOTE: versions listed below have been tested
- A GEVCU board. This version supports GEVCU7 hardware (the one with Teensy MicroMod)
- Arduino IDE 1.8.16^
- TeensyDuino 1.58.1 (https://www.pjrc.com/teensy/td_download.html)
- TeensyTimerTool 1.3.1 (https://github.com/luni64/TeensyTimerTool)
- ArduinoJson 6.21.2 (https://github.com/bblanchon/ArduinoJson)
- WDT_T4 0.1.0 (https://github.com/tonton81/WDT_T4)
- All other libraries are bundled with TeensyDuino

Installation:
1. Clone the git repository
2. Add this link (https://www.pjrc.com/teensy/package_teensy_index.json) as an "Additional boards manager URLs"
3. Download TeensyDuino via the Arduino Board Manager
4. Download TeensyTimerTool and ArduinoJson via the Arduino Library Manager
5. Download WDT_T4 via git to your sketchbook libraries directory
  * Windows: `%USERPROFILE%\Documents\Arduino\libraries\`
  * Mac/Unix: `~/Documents/Arduino/libraries/`

Arduino Board Configuration:
* Selected Board: Teensy MicroMod (Teensy)
* Selected Port: ... Dual Serial (Teensy MicroMod)
* USB Type: Dual Serial
* CPU Speed: 600 MHz
* Optimize: Faster
* Keyboard Layout: US English

Tips:
* Set the Arduino output path to speed up compilation times (e.g. ./build)
* View the [ExampleDevice.cpp](docs/ExampleDevice.cpp) file to understand how to support a new device


The canbus is supposed to be terminated on both ends of the bus. If you are testing with a DMOC and GEVCU then you've got two devices, each on opposing ends of the bus. GEVCU7 hardware is selectively terminated. By default it is not terminated but this can be solved by soldering the appropriate solder jumper

## License

This software is MIT licensed:

Copyright (c) 2021 Collin Kidder, Michael Neuweiler, Charles Galpin, Jack Rickard

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Useful Links
* [GEVCU7 Operation Manual PDF](docs/GEVCU_7.00_Manual.pdf)
* Google Drive Links
  * [Brusa Charger](https://drive.google.com/drive/u/0/folders/1DeVJPek3m3y8n4FABni9ARRhTu9wiDik)
  * [Orion BMS](https://drive.google.com/drive/u/0/folders/1ZqXBEEOgR6AhwdcV8Tug0shH_WJ5AyDg)
  * [Bamocar Motor Controller](https://drive.google.com/drive/u/0/folders/1WSYGdv5YnkaVZ9nAUO3y6nDQfeHA4u8Z)