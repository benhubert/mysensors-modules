HYT221 module for mysensors
=============================
This is a small temperature and humidity sensor module for the mysensors.org
IoT network.

Author: Benjamin Hubert
Distributed under GPLv3

Hardware requirements
-----------------------
This code requires:

  * Arduino Nano or Arduino Pro Mini
  * nrf24l01 wireless chip
  * HYT221 humidity and temperature sensor
  * Capacitor (4.7µ - 47µF, optional)

Check out the [README of MyBattery](tempsensor/src/mybattery/README.md) for
hardware that is required for reading the battery power.

This code was tested on an Arduino Nano but should also run on an Arduino Uno
or an Arduino Pro Mini.

Wiring
--------
Check out [the mysensors.org website][1] to find out how to connect the
nrf24l01 wireless chip. Add the optional capacitor between 3.3V and GND.

The HYT221 module must be connected as follows:

| Arduino | HYT221 | Description   |
| ------- | ------ | ------------- |
|   A4    |  SDA   | Data          |
|   GND   |  GND   | Ground        |
|   3v3   |  VDD   | 3.3 V (or 5V) |
|   A5    |  SCL   | Clock         |

For details about the wiring setup and the I2C protocol check out the
[documentation for HYT221][2].

[1]: https://www.mysensors.org/build/connect_radio
[2]: https://www.ist-ag.com/sites/default/files/AHHYTM_E.pdf

