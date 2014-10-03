#include <OneWire.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 11
#define DIO 10

// The amount of time (in milliseconds) between tests
#define TEST_DELAY 10

#define TARGET_MIN    25
#define TARGET        28

// Relay pins
#define RELAY1   7
#define RELAY2   8

TM1637Display display(CLK, DIO);
OneWire thermometer(2); // ds18s20 thermometer
int k = 0;
float temperature = 0;

void setup()
{
  Serial.begin(9600);
  display.setBrightness(0x0f);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
}

void loop()
{
  // Show decimal numbers with/without leading zeros
  int t = (int)(temperature * 100);
  display.showNumberDec(t, true, 4, 0);
  runThermo();
  if (temperature < TARGET) {
    digitalWrite(RELAY1, HIGH);
  } else {
    digitalWrite(RELAY1, LOW);
  }
  if (temperature < TARGET_MIN) {
    digitalWrite(RELAY2, HIGH);
  } else {
    digitalWrite(RELAY2, LOW);
  }
  delay(TEST_DELAY);
}


/* *********** */
/* THERMOMETER */
/* *********** */

byte data[12];
byte addr[8];
byte present = 0;

long nextThermoEvent = 0;
byte thermoState = 0; // 0 before setup, 1 before read, 2 processing

void runThermo() {
  Serial.println("Running thermo");
  if (nextThermoEvent < millis()) {
    nextThermoEvent = millis() + 1000;
    if (thermoState == 0) {
      Serial.println("Initializing thermo");
      thermoSetup();
    } else if (thermoState == 1){
      Serial.println("Beginning read");
      initThermoRead();
    } else if (thermoState == 2){
      Serial.println("GEtting back results");
      readThermo();
    } else {
      Serial.println("Just do nothing.");
    }
  }
}

void thermoSetup() {
  byte i;
  if ( !thermometer.search(addr)) {
    thermometer.reset_search();
    Serial.println("Search reset");
    return;
  }
  Serial.print("R=");
  for( i = 0; i < 8; i++) {
    Serial.println(addr[i], HEX);
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }
  if ( addr[0] == 0x10) {
    Serial.println("Device is a DS18S20 family device.");
  }
  else if ( addr[0] == 0x28) {
    Serial.println("Device is a DS18B20 family device.");
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

/* *********** */
/*   DISPLAY   */
/* *********** */

void clearDisplay() {
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setSegments(data);
}

