# Double PN532 Arduino

This project demonstrates how to use two PN532 NFC modules using an Arduino. It reads Mifare and Felica cards, and displays the card information on the serial monitor.

I use it mainly on my ITGmania cards server project: [ITGmania Cards Server](https://github.com/Feiryn/itgmania_cards_server).

## Hardware Requirements

- Arduino board with at least 1 HSU and 1 I2C interface
- 2 PN532 NFC modules

## Wiring

1. Connect the first PN532 module to the Arduino using the HSU interface:
   - VCC to 3.3V or 5V (depending on your module)
   - GND to GND
   - TX and RX to the one specified in the code (e.g. `SoftwareSerial SWSerial1(3, 2); // TX, RX`)
2. Connect the second PN532 module to the Arduino using the I2C interface:
   - VCC to 3.3V or 5V (depending on your module)
   - GND to GND
   - SDA to A4 (or the SDA pin on your Arduino)
   - SCL to A5 (or the SCL pin on your Arduino)

Don't forget to switch the jumpers on the PN532 modules to select the correct communication mode (HSU for the first module and I2C for the second module).

## Software

The project uses this library: [PN532](https://github.com/elechouse/PN532). Make sure to install it in your Arduino IDE.
