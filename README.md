Gas consumption sensor for mysensors
======================================
Small sensor that tracks the gas consumption using the magnetic signal of a
gas meter.

Author: Benjamin Hubert
Distributed under GPLv3

Hardware requirements
-----------------------
This code requires:

  * Arduino Nano or Arduino Pro Mini
  * nrf24l01 wireless chip
  * Reed switch

This code was tested on an Arduino Nano but should also run on an Arduino Uno
or an Arduino Pro Mini.

Gas meter information
-----------------------
The gas meter in use is a GMT GT4. This unit triggers a magnetic pulse near the
number display. 1 impulse means a gas consumption of 0.1m³ gas.

Wiring
--------
Check out [the mysensors.org website][1] to find out how to connect the
nrf24l01 wireless chip.

The reed switch must be connected as follows:

| Arduino | Reed Switch |
| ------- | ----------- |
|   D3    | one end     |
|   GND   | other end   |

[1]: https://www.mysensors.org/build/connect_radio

