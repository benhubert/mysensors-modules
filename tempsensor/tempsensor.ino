/*
 * This file is part of tempsensor-hyt221 for mysensors.
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

// Enable debug prints
#define MY_DEBUG

// Enable REPORT_BATTERY_LEVEL to measure battery level and send changes to gateway
//#define REPORT_BATTERY_LEVEL

// Enable and select radio type attached 
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
//#define MY_RS485
#define MY_RX_MESSAGE_BUFFER_FEATURE
#define MY_RF24_IRQ_PIN 2

#include <MySensors.h>  

#define CHILD_ID_HUM  0
#define CHILD_ID_TEMP 1

static bool metric = true;

// Sleep time between sensor updates (in milliseconds)
static const uint64_t MEASURE_WAIT = 15000;
static const uint64_t UPDATE_INTERVAL = 30000-MEASURE_WAIT;

#include "hyt221.h"
static HYT221 sensor;

#ifdef REPORT_BATTERY_LEVEL
#include <Vcc.h>
static uint8_t oldBatteryPcnt = 200;  // Initialize to 200 to assure first time value will be sent.
const float VccMin        = 1.8;      // Minimum expected Vcc level, in Volts: Brownout at 1.8V    -> 0%
const float VccMax        = 2.0*1.6;  // Maximum expected Vcc level, in Volts: 2xAA fresh Alkaline -> 100%
const float VccCorrection = 1.0;      // Measured Vcc by multimeter divided by reported Vcc
static Vcc vcc(VccCorrection); 
#endif

void presentation()  
{ 
  // Send the sketch info to the gateway
  sendSketchInfo("TemperatureAndHumidity", "1.0");

  // Present sensors as children to gateway
  present(CHILD_ID_HUM,  S_HUM,  "Humidity");
  present(CHILD_ID_TEMP, S_TEMP, "Temperature");

  metric = getControllerConfig().isMetric;
}

void setup()
{
  while (not sensor.begin()) {
    Serial.println(F("Sensor not detected!"));
    sleep(5000);
  }
  Serial.println(F("Sensor initialized"));
}


void loop()      
{
  // Start measurement
  sensor.start_measure();
  sleep(MEASURE_WAIT);
  
  // Read temperature & humidity from sensor.
  const float temperature = float( metric ? sensor.read_celsius_hundredths() : sensor.read_fahrenheit_hundredths() ) / 100.0;
  const float humidity    = float( sensor.read_humidity_percent_hundredths() ) / 100.0;
  
#ifdef MY_DEBUG
  Serial.print(F("Temp "));
  Serial.print(temperature);
  Serial.print(metric ? 'C' : 'F');
  Serial.print(F("\tHum "));
  Serial.println(humidity);
#endif

  static MyMessage msgHum( CHILD_ID_HUM,  V_HUM );
  static MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

  if (temperature > -327.0 && humidity >= 0.0) {
    send(msgTemp.set(temperature, 2));
    send(msgHum.set(humidity, 2));
  }

#ifdef REPORT_BATTERY_LEVEL
  const uint8_t batteryPcnt = static_cast<uint8_t>(0.5 + vcc.Read_Perc(VccMin, VccMax));

#ifdef MY_DEBUG
  Serial.print(F("Vbat "));
  Serial.print(vcc.Read_Volts());
  Serial.print(F("\tPerc "));
  Serial.println(batteryPcnt);
#endif

  // Battery readout should only go down. So report only when new value is smaller than previous one.
  if ( batteryPcnt < oldBatteryPcnt )
  {
      sendBatteryLevel(batteryPcnt);
      oldBatteryPcnt = batteryPcnt;
  }
#endif

  // Sleep until next update to save energy
  sleep(UPDATE_INTERVAL); 
}
