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
#define SKETCH_VERSION "0.2"

#include "MySettings.h"
#include <MySensors.h>

// ID of the sensor child (only one sensor, so 0)
#define CHILD_ID 0

// Pin where the reed switch is connected
#define SENSOR_PIN 3

// Pin where the button is connected
#define BUTTON_PIN 2

// Pin where the LED is connected
#define LED_PIN 7
#define LED_FLASH_MILLIS 300

// The pulse factor
// Number of impulses per m3 of your meter
#define PULSE_FACTOR 10

// Check for pulse event every 30 seconds
#define READ_FREQUENCY_MILLIS 30000

// ---------------------------------------------------------

#define STATE_INITIALIZE 0
#define STATE_SENSE_AND_SEND 1
#define STATE_WAIT_FOR_OVERRIDE 2

#include "src/mybattery/mybattery.h"
static MyBattery mybattery;

// current value of the gas meter
volatile byte state = STATE_INITIALIZE;
volatile unsigned long stayInStateUntil = 0;
volatile float counter = 0.0;
volatile float counterOverride = -1.0;
volatile bool pulseDetected = false;

// sends the current volume
MyMessage volumeMessage(CHILD_ID, V_VOLUME);

void setup() {
  initializeLed();
  initializeButton();
  initializeSensor();
}

void initializeLed() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, false);
}

void initializeButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // clear interrupt flag (avoids pulse at startup)
  EIFR |= (1 << digitalPinToInterrupt(BUTTON_PIN));
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, FALLING);
}

void initializeSensor() {
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  // clear interrupt flag (avoids pulse at startup)
  EIFR |= (1 << digitalPinToInterrupt(SENSOR_PIN));
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onPulse, FALLING);
}

void presentation() {
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  present(CHILD_ID, S_GAS, "Gas consumption");
}

void loop() {
  if (state == STATE_INITIALIZE) {
    digitalWrite(LED_PIN, true);

    // send request every 10 seconds
    if (millis() > stayInStateUntil) {
      #ifdef MY_DEBUG
        Serial.println(F("Requesting count from gateway"));
      #endif
      request(CHILD_ID, V_VOLUME);
      stayInStateUntil = millis() + 10000;
    }
    
    if (counterOverride >= 0.0) {
      #ifdef MY_DEBUG
        Serial.print(F("Initializing count with: "));
        Serial.println(counterOverride);
      #endif
      counter += counterOverride;
      counterOverride = -1.0;
      state = STATE_SENSE_AND_SEND;
    }
  } else if (state == STATE_WAIT_FOR_OVERRIDE) {
    digitalWrite(LED_PIN, true);

    if (counterOverride >= 0.0) {
      #ifdef MY_DEBUG
        Serial.print(F("Overriding count with: "));
        Serial.println(counterOverride);
      #endif
      counter = counterOverride;
      counterOverride = -1.0;
      state = STATE_SENSE_AND_SEND;
    }

    if (millis() > stayInStateUntil) {
      state = STATE_SENSE_AND_SEND;
    }
  } else if (state == STATE_SENSE_AND_SEND) {
    digitalWrite(LED_PIN, false);
    
    if (pulseDetected) {
      #ifdef MY_DEBUG
        Serial.println(F("Pulse detected"));
      #endif
      counter += 1.0 / PULSE_FACTOR;
      flashLed(1);
      pulseDetected = false;
    }
    
    #ifdef MY_DEBUG
      Serial.print(F("Sending count: "));
      Serial.println(counter);
    #endif
    send(volumeMessage.set(counter, 1));
    sleep(READ_FREQUENCY_MILLIS);
  
    mybattery.readAndReportBatteryLevel();
  }
}

void onPulse() {
  pulseDetected = true;
}

void onButtonPressed() {
  state = STATE_WAIT_FOR_OVERRIDE;
  stayInStateUntil = millis() + 30000;
}

void receive(const MyMessage &message) {
  bool processed = false;
  if (message.type == V_VOLUME) {
    if (state == STATE_INITIALIZE || state == STATE_WAIT_FOR_OVERRIDE) {
      counterOverride = message.getFloat();
      digitalWrite(LED_PIN, false);
      processed = true;
    }
  }
  if (!processed) {
    #ifdef MY_DEBUG
      Serial.print(F("Received unknown message: "));
      Serial.println(message.type);
    #endif
  }
}

void flashLed(int times) {
  bool ledState = digitalRead(LED_PIN);

  digitalWrite(LED_PIN, false);
  wait(LED_FLASH_MILLIS);
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, false);
    wait(LED_FLASH_MILLIS);
    digitalWrite(LED_PIN, true);
    wait(LED_FLASH_MILLIS);
  }
  digitalWrite(LED_PIN, false);
  wait(LED_FLASH_MILLIS);
  wait(LED_FLASH_MILLIS);
  
  digitalWrite(LED_PIN, ledState);
}
