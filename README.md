SI7021 module for mysensors
=============================
This is a small temperature and humidity sensor module for the mysensors.org
IoT network.

The code is based on the [mysensors SI7021 example][1] with some small
adoptions.

Author: Benjamin Hubert
Distributed under GPLv3

[1] https://www.mysensors.org/build/humidity_si7021

Hardware requirements
-----------------------
This code requires:

  * Arduino Nano or Arduino Pro Mini
  * nrf24l01 wireless chip
  * SI7021 humidity and temperature sensor breakout board

This code was tested on an Arduino Pro Mini but should also run on an Arduino
Uno or an Arduino Nano.

Wiring
--------
Check out [the mysensors.org website][1] to find out how to connect the
nrf24l01 wireless chip.

The SI7021 module must be connected as follows:

| Arduino | HYT221 | Description   |
| ------- | ------ | ------------- |
|   A4    |  SDA   | Data          |
|   GND   |  GND   | Ground        |
|   3v3   |  VDD   | 3.3 V (or 5V) |
|   A5    |  SCL   | Clock         |

[1]: https://www.mysensors.org/build/connect_radio

