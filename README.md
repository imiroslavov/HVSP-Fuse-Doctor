
# HVSP Fuse Doctor

The story began a long time ago. As of the moment that I am writing this README.md almost 21 years passed since then. 
I was really passionate to learn how to design and make my own hardware so I started learning how to make my own schematics, PCBs and how to program microcontrollers. 
Unfortunately I didn't knew how to properly program fuses and what was the difference between a crystal and clock oscillator so I ended up with a pile of "dead" microcontrollers. 
Back in these days I had a self made High-voltage programmer (The almighty "ByteBlaster+". I will post schematics and board design on another repository) but nowadays I have no place to plug it in since LPT ports are long time out of use in modern computing. 
I was developing a Low-ohm continuity tester with ATtiny2313 and accidentally bricked the MCU in my personal "old-school style" way (I've pulled my beard at least 1cm longer) and guess what... 
It was the last one in the box that was alive. So the journey began. 
I've ended up finding a lot of schematics and information, but most of it was related to using Arduino which for me was just a big "Ehh!". 
I found a few blog posts which helped me to design the device to be dependent on the power supply only (credits on the end of the page).

## Pinout

| PIN  | AVR | HVSP |
| ------------- | ------------- | ------------- |
| 1 | PC5 | **12V / RST** |
| 2 | PC0 | **SDI** |
| 3 | PC1 | **SII** |
| 4 | PC2 | **SDO** |
| 5 | PC3 | **SCI** |
| 6 | PC4 | **5V / VCC** |
| 7 | GND | **GND** |

## Schematics
This is the base schematic. Version 2 contains additional header connected to PD1 and GND which is used for RS232 communication and can be omitted. Thus the connector it's not shown here because it is optional.

![Schematic](https://raw.githubusercontent.com/imiroslavov/HVSP-Fuse-Doctor/refs/heads/main/assets/schematic.png "Schematic")

## Board prototype Version 1 (dual power supply)
|                                                    Bottom layer                                                    |                                                              Top view                                                               |
|:------------------------------------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------------------:|
| ![Eagle](https://raw.githubusercontent.com/imiroslavov/HVSP-Fuse-Doctor/refs/heads/main/assets/v1/eagle.png "Eagle") | ![Fritzing](https://raw.githubusercontent.com/imiroslavov/HVSP-Fuse-Doctor/refs/heads/main/assets/v1/fritzing.png "Fritzing") |

Please note that the main.sch has additional sheet containing the mounting pads and connectors not shown in the schematics diagram above. They are there, but feel free to change them to make your own design (I've used one of those power supplies for external SATA disk drive readers that provide 5 and 12 volts simultaneously (DIN6)). 

## Notes
If the system still does not answer for a simple ISP programmer, that means it has damaged hardware SPI, other damage or bad EEPROM.

Thanks to Peter Fleury at http://www.peterfleury.epizy.com/ for the original schematics and code. I've changed it of course but it is based on his work.
