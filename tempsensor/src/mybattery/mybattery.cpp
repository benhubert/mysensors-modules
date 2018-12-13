#include "Arduino.h"
#include "mybattery.h"

#include "../../MySettings.h"
#include <core/MySensorsCore.h>

#define MYBATTERY_FORCE_SEND_EVERY_NTH_CALL 10

int batterySensePin = 0;
float batteryMaxVoltage = 3.44;
float batteryVoltsPerBit = 0.003363075;
int lastBatteryPercentage = -1;
int callCount = 0;

MyBattery::MyBattery() {
}

void MyBattery::setup(int sensePin, float maxVoltage) {
	batterySensePin = sensePin;
	batteryMaxVoltage = maxVoltage;
	batteryVoltsPerBit = maxVoltage / 1023;
	analogReference(INTERNAL);

#ifdef MY_DEBUG
	Serial.print(F("MyBattery sense pin: "));
	Serial.println(batterySensePin);
	Serial.print(F("MyBattery max voltage: "));
	Serial.print(batteryMaxVoltage);
	Serial.println(F("V"));
	Serial.print(F("MyBattery volts per bit: "));
	Serial.print(batteryVoltsPerBit);
	Serial.println(F("V"));
#endif
}

void MyBattery::readAndReportBatteryLevel() {
	int sensorValue = analogRead(batterySensePin);
	int batteryPercentage = sensorValue / 10;

#ifdef MY_DEBUG
	float batteryVoltage = sensorValue * batteryVoltsPerBit;
	Serial.print(F("MyBattery sensor value: "));
	Serial.println(sensorValue);
	Serial.print(F("MyBattery battery voltage: "));
	Serial.print(batteryVoltage);
	Serial.print(F(" / "));
	Serial.println(batteryMaxVoltage);
	Serial.print(F("MyBattery battery percentage: "));
	Serial.println(batteryPercentage);
	Serial.print(F("MyBattery battery percentage before: "));
	Serial.println(lastBatteryPercentage);
	Serial.print(F("MyBattery call count: "));
	Serial.println(callCount);
#endif

	if (batteryPercentage != lastBatteryPercentage || callCount % MYBATTERY_FORCE_SEND_EVERY_NTH_CALL == 0) {
		sendBatteryLevel(batteryPercentage);
		lastBatteryPercentage = batteryPercentage;
	}

	callCount++;
}

