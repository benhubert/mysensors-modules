#ifndef mybattery_h
#define mybattery_h

/**
 * This library reads and reports the voltage of a mysensors network node.
 *
 * It is primarily intended for the use with a set of 2 AA batteries powering
 * an Arduino Mini Pro node.
 */
class MyBattery
{
  public:

    MyBattery();

    /**
     * Configures the arduino for measuring the battery voltage. This must be
     * called at startup.
     *
     * Note that this will set the analog reference to 1.1V.
     *
     * @param sensePin the analog pin where the voltage is read (typically A0)
     * @param maxBatteryVoltage the maximum voltage of the battery (i.e. the
              battery voltage when sensePin reads 1.1V)
     */
    void setup(int sensePin, float maxBatteryVoltage);

    /**
     * Reads the battery level and sends it over the mysensors network.
     */
    void readAndReportBatteryLevel(void (*reportFunction)(int));

};

#endif

