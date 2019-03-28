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

class HYT221 {
  
  public:
    HYT221();
    /**
     * Initializes the sensor.
     */
    bool begin();
    /**
     * Starts a measurement. This must be done >12 seconds before reading the
     * results.
     */
    void start_measure();
    /**
     * Returns the last measured temperature in degrees celsius (x100).
     * Example: 1522 for 15,22°C
     * To ensure that this value is up to date, call start_measure() at least
     * 12 seconds before.
     */
    int read_celsius_hundredths();
    /**
     * Returns the last measured temperature in degrees fahrenheit (x100).
     * Example: 5912 for 59,12°F
     * To ensure that this value is up to date, call start_measure() at least
     * 12 seconds before.
     */
    int read_fahrenheit_hundredths();
    /**
     * Returns the last measured humidity in percent (x100).
     * Example: 2560 for 25,6%
     * To ensure that this value is up to date, call start_measure() at least
     * 12 seconds before.
     */
    int read_humidity_percent_hundredths();
    
  private:
    /**
     * Tells the HYT221 to start a measurement. This is done by simply starting
     * a transmission to the write address of the chip.
     */
    void _send_measurement();
    /**
     * Reads data from I2C if available.
     */
    void _read_data_if_available();
    int _calc_celsius_hundredths(byte* data);
    int _calc_fahrenheit_hundredths(byte* data);
    int _calc_humidity_percent_hundredths(byte* data);

};
