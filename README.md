Gas consumption sensor for mysensors
======================================
Small sensor that tracks the gas consumption using the magnetic signal of a
gas meter.

The meter comes with a status LED and a push button. When the push button is
pressed, it waits for the count from the gateway. While waiting, the LED is on.
As soon as it received a count, the LED turns off.

When a pulse has been detected, the LED flashes one time.

Author: Benjamin Hubert
Distributed under GPLv3

Hardware requirements
-----------------------
This code requires:

  * Arduino Nano or Arduino Pro Mini
  * nrf24l01 wireless chip
  * Reed switch
  * Push button
  * LED with resistor

This code was tested on an Arduino Mini Pro but should also run on an Arduino
Uno or an Arduino Nano.

Gas meter information
-----------------------
The gas meter in use is a GMT GT4. This unit triggers a magnetic pulse near the
number display. 1 impulse means a gas consumption of 0.1m³ gas.

Wiring
--------
Check out [the mysensors.org website][1] to find out how to connect the
nrf24l01 wireless chip. Do not connect the IRQ pin, you will need it for
the button.

The reed switch must be connected as follows:

| Arduino | Reed Switch |
| ------- | ----------- |
|   D3    | one end     |
|   GND   | other end   |

The push button must be connected to:

| Arduino | Push Button |
| ------- | ----------- |
|   D2    | one end     |
|   GND   | other end   |

The LED (with a resistor in series) must be connected to:

| Arduino | LED         |
| ------- | ----------- |
|   D7    | anode   (+) |
|   GND   | cathode (-) |

[1]: https://www.mysensors.org/build/connect_radio

