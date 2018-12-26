SI7021 module for mysensors
=============================
This is a small temperature and humidity sensor module for the mysensors.org
IoT network.

The code is based on the [mysensors SI7021 example][1] with some small
adoptions. I decided to use SI7021 based on the test results of
[this great article][2].

Author: Benjamin Hubert
Distributed under GPLv3

[1]: https://www.mysensors.org/build/humidity_si7021
[2]: http://www.kandrsmith.org/RJS/Misc/hygrometers.html

Hardware requirements
-----------------------
This code requires:

  * Arduino Nano or Arduino Pro Mini
  * nrf24l01 wireless chip
  * SI7021 humidity and temperature sensor breakout board
  * Capacitor (4.7µ - 47µF, optional)

Check out the [README of MyBattery](tempsensor/src/mybattery/README.md) for
hardware that is required for reading the battery power.

This code was tested on an Arduino Pro Mini but should also run on an Arduino
Uno or an Arduino Nano.

Wiring
--------
Check out [the mysensors.org website][3] to find out how to connect the
nrf24l01 wireless chip. Add the optional capacitor between 3.3V and GND.

The SI7021 module must be connected as follows:

| Arduino | HYT221 | Description   |
| ------- | ------ | ------------- |
|   A4    |  SDA   | Data          |
|   GND   |  GND   | Ground        |
|   3v3   |  VDD   | 3.3 V (or 5V) |
|   A5    |  SCL   | Clock         |

[3]: https://www.mysensors.org/build/connect_radio

