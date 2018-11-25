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

#include "Arduino.h"
#include "hyt221.h"
#include <Wire.h>

#define HYT221_I2C_ADDR 0x28

#define HYT221_STATUS_UNINITIALIZED 0
#define HYT221_STATUS_STANDBY 1
#define HYT221_STATUS_MEASURING 2
#define HYT221_TEMP_UNKNOWN -32768
#define HYT221_HUMI_UNKNOWN -32768

bool _hyt_exists = false;
byte _hyt_status = 0;
byte _hyt_measure_start = 0;
int _hyt_temp_celsius_hundredths = HYT221_TEMP_UNKNOWN;
int _hyt_temp_fahrenheit_hundredths = HYT221_TEMP_UNKNOWN;
int _hyt_humi_percent_hundredths = HYT221_HUMI_UNKNOWN;

HYT221::HYT221() {
}

bool HYT221::begin() {
  if (_hyt_status == HYT221_STATUS_UNINITIALIZED) {
    Wire.begin();
    Wire.beginTransmission(HYT221_I2C_ADDR);
    if (Wire.endTransmission() == 0) {
      _hyt_exists = true;
      _hyt_status = HYT221_STATUS_STANDBY;
    }
  }
  return _hyt_exists;
}

void HYT221::start_measure() {
  if (_hyt_status == HYT221_STATUS_STANDBY) {
    _hyt_status = HYT221_STATUS_MEASURING;
    _send_measurement();
  }
}

int HYT221::read_celsius_hundredths() {
  _read_data_if_available();
  return _hyt_temp_celsius_hundredths;
}

int HYT221::read_fahrenheit_hundredths() {
  _read_data_if_available();
  return _hyt_temp_fahrenheit_hundredths;
}

int HYT221::read_humidity_percent_hundredths() {
  _read_data_if_available();
  return _hyt_humi_percent_hundredths;
}

void HYT221::_send_measurement() {
  Wire.beginTransmission(HYT221_I2C_ADDR);
  Wire.endTransmission(HYT221_I2C_ADDR);
}

void HYT221::_read_data_if_available() {
  if (_hyt_status == HYT221_STATUS_MEASURING) {
    Wire.requestFrom(HYT221_I2C_ADDR, 4);
    if (Wire.available() == 4) {
      byte data[4];
      data[0] = Wire.read();
      data[1] = Wire.read();
      data[2] = Wire.read();
      data[3] = Wire.read();
      _hyt_temp_celsius_hundredths = _calc_celsius_hundredths(data);
      _hyt_temp_fahrenheit_hundredths = _calc_fahrenheit_hundredths(data);
      _hyt_humi_percent_hundredths = _calc_humidity_percent_hundredths(data);
      _hyt_status = HYT221_STATUS_STANDBY;
    }
  }
}

int HYT221::_calc_celsius_hundredths(byte* data) {
  int raw = data[2] << 6 | data[3] >> 2;
  return raw * (16500.0 / 16383.0) - 4000;
}

int HYT221::_calc_fahrenheit_hundredths(byte* data) {
  int celsius = _calc_celsius_hundredths(data);
  return 1.8 * celsius + 32;
}

int HYT221::_calc_humidity_percent_hundredths(byte* data) {
  int raw = data[0] << 8 | data[1];
  raw &= 0x3fff;
  return raw * (10000.0 / 16383.0);
}
