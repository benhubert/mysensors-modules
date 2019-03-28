MyBattery
===========
Library for reading the battery health of MySensors nodes. This code is
primarily designed for measuring the voltage of two AA batteries powering a
mysensors network node.

Author: Benjamin Hubert, code@benjaminhubert.at


Hardware requirements
-----------------------
This code requires for measuring the voltage of two AA batteries (besides all
the other parts of the node):

  * 1MΩ resistor (R1)
  * 470KΩ resistor (R2)
  * 0.1uF capacitor


Wiring
--------
Connect all parts as follows:


               (+)──────[R1 1MΩ]─────┐
    2 AA batteries (3V)              ├─── A0 (Arduino Mini Pro)
               (-)──┬──[R2 470KΩ]──┬─┘
                    │              │
                    └────(0.1uF)───┘

