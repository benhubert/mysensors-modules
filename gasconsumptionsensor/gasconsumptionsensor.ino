/*
 * This file is part of gasconsumptionsensor for mysensors.
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this code.  If not, see <https://www.gnu.org/licenses/>.
 */

#define SKETCH_NAME "gasconsumption"
#define SKETCH_VERSION "0.1"

#include "MySettings.h"
#include <MySensors.h>

// ID of the sensor child (only one sensor, so 0)
#define CHILD_ID 0

// Pin where the reed switch is connected
#define DIGITAL_INPUT_SENSOR 3

// Pin where the LED is connected
#define LED_PIN 13

// The pulse factor
// Number of impulses per m3 of your meter
#define PULSE_FACTOR 10

// Inform the gateway every 30 seconds
uint32_t SEND_FREQUENCY = 30000;

// ---------------------------------------------------------

#include "src/mybattery/mybattery.h"
static MyBattery mybattery;

// current value of the gas meter
volatile float counter = 0.0;
volatile bool waitForCountMessage = true;
volatile unsigned long lastPulse = 0;

// sends the current volume
MyMessage volumeMessage(CHILD_ID, V_VOLUME);

void setup() {
  pinMode(DIGITAL_INPUT_SENSOR, INPUT_PULLUP);
  // clear interrupt flag (avoids pulse at startup)
  EIFR |= (1 << digitalPinToInterrupt(DIGITAL_INPUT_SENSOR));
  attachInterrupt(digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), onPulse, FALLING);
}

void presentation() {
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  present(CHILD_ID, S_GAS, "Gas consumption");
}

void loop() {
  #ifdef MY_DEBUG
    Serial.print(F("Counter: "));
    Serial.println(counter);
    Serial.print(F("Waiting for count message: "));
    Serial.println(waitForCountMessage ? F("true") : F("false"));
  #endif

  if (!waitForCountMessage) {
    send(volumeMessage.set(counter, 1));
    mybattery.readAndReportBatteryLevel();
    sleep(SEND_FREQUENCY);
  } else {
    request(CHILD_ID, V_VOLUME);
    delay(5000);
  }
}

void onPulse() {
  #ifdef MY_DEBUG
    Serial.println(F("Pulse detected"));
  #endif
  counter += 1.0 / PULSE_FACTOR;
}

void receive(const MyMessage &message) {
  if (message.type == V_VOLUME) {
    if (waitForCountMessage) {
      counter = message.getFloat();
      waitForCountMessage = false;
      #ifdef MY_DEBUG
        Serial.print(F("Received volume count. Count is now: "));
        Serial.println(counter);
      #endif
    } else {
      #ifdef MY_DEBUG
        Serial.print(F("Received volume count but not waiting for it. Ignoring."));
      #endif
    }
  } else {
    #ifdef MY_DEBUG
      Serial.print(F("Received unknown message of type "));
      Serial.println(message.type);
    #endif
  }
}
