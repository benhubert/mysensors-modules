/*
 * This file is part of tempsensor-si7021 for mysensors.
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

#define SKETCH_NAME "humi_si7021"
#define SKETCH_VERSION "1.0"

#include "MySettings.h"
#include <MySensors.h>

#define CHILD_ID_HUM  0
#define CHILD_ID_TEMP 1

static bool metric = true;

// Sleep time between sensor updates (in milliseconds)
static const uint64_t MEASURE_WAIT = 15000;
static const uint64_t UPDATE_INTERVAL = 30000-MEASURE_WAIT;

#include "src/si7021/SI7021.h"
static SI7021 sensor;

#include "src/mybattery/mybattery.h"
static MyBattery mybattery;

void presentation()  
{ 
  // Send the sketch info to the gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Present sensors as children to gateway
  present(CHILD_ID_HUM,  S_HUM,  "Humidity");
  present(CHILD_ID_TEMP, S_TEMP, "Temperature");

  metric = getControllerConfig().isMetric;
}

void setup()
{
  mybattery.setup(A0, 3.44);
  while (not sensor.begin()) {
    Serial.println(F("Sensor not detected!"));
    sleep(5000);
  }
  Serial.println(F("Sensor initialized"));
}


void loop()      
{  
  // Read temperature & humidity from sensor.
  const float temperature = float(metric ? sensor.getCelsiusHundredths() : sensor.getFahrenheitHundredths()) / 100.0;
  const float humidity    = float(sensor.getHumidityBasisPoints()) / 100.0;

#ifdef MY_DEBUG
  Serial.print(F("Temp "));
  Serial.print(temperature);
  Serial.print(metric ? 'C' : 'F');
  Serial.print(F("\tHum "));
  Serial.println(humidity);
#endif

  static MyMessage msgHum(CHILD_ID_HUM, V_HUM);
  static MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

  send(msgTemp.set(temperature, 2));
  send(msgHum.set(humidity, 2));

  mybattery.readAndReportBatteryLevel(&sendBatteryLevel);

  // Sleep until next update to save energy
  sleep(UPDATE_INTERVAL); 
}
