#include "Arduino.h"
#include "Wire.h"
#include "OneWire.h"

OneWire thermometer;// ds18s20 thermometer
byte present = 0;
byte data[12];
byte addr[8];
float temperature = 0;

Thermometer::Thermometer(int pin) {
	thermometer(pin);
}

void setup() {
  thermoSetup();
}

void loop() {
  runThermo();
  refreshDisplay();
}
/* *********** */
/* THERMOMETER */
/* *********** */

long nextThermoEvent = 0;
byte thermoState = 0; // 0 before setup, 1 before read, 2 processing

void runThermo() {
  if (nextThermoEvent < millis()) {
    Serial.print("thermoState: ");
    Serial.println(thermoState);
    nextThermoEvent = millis() + 1000;
    if (thermoState == 0) {
      thermoSetup();
    } else if (thermoState == 1){
      initThermoRead();
    } else if (thermoState == 2){
      readThermo();
    }
  }
}

void thermoSetup() {
  byte i;
  if ( !thermometer.search(addr)) {
    thermometer.reset_search();
    return;
  }
  Serial.print("R=");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.print("CRC is not valid!\n");
    return;
  }
  if ( addr[0] == 0x10) {
    Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
    Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
    Serial.print("Device family is not recognized: 0x");
    Serial.println(addr[0],HEX);
    return;
  }
  thermoState = 1;
}

void initThermoRead() {
  thermometer.reset();
  thermometer.select(addr);
  thermometer.write(0x44,1);
  thermoState = 2;
}

void readThermo() {
  present = thermometer.reset();
  thermometer.select(addr);    
  thermometer.write(0xBE); 
  byte i;
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = thermometer.read();
  }

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  temperature = tempRead / 16;
  thermoState = 1;
}
