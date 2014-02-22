#ifndef Thermometer_h
#define Thermometer_h

#include "Arduino.h"
#include "Wire.h"
#include "OneWire.h"

class Thermometer {
	#define BEFORE_SETUP  0
	#define BEFORE_READ  1
	#define READ_STARTED  2

	private:
		byte present = 0;
		byte data[12];
		byte addr[8];
		float temperature = 0;
		long nextThermoEvent = 0;
		byte thermoState = 0; // 0 before setup, 1 before read, 2 processing
		
	public:
		void setupThermometer(int pin);
		float getTemperature();
	private:
		void initThermoRead();
		void readThermo();
}
#endif