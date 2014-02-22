#include <Time.h>
#include <DS1307RTC.h>

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <OneWire.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message

#define versionCode "v0.6.6"

OneWire thermometer(7); // ds18s20 thermometer
LiquidCrystal lcd(13, 12, 3, 8, 9, 10, 11);
byte present = 0;
byte data[12];
byte addr[8];
float temperature = 0;

// Keypad
const byte rows = 4;
const byte cols = 4;

char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {
  4,5,6,A0};
byte colPins[cols] = {
  A1,A2,A3,2}; // Digital 13 and analog 6-7 are not good pins to use.

char key;
int keypress = 0;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() {
  Serial.begin(9600);
  lcd.begin(20,4);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } 
  else {
    Serial.println("RTC has set the system time");
  }
  // Set up clock
  /*
  time_t t = processSyncMessage();
   if(t >0)
   {
   RTC.set(t);   // set the RTC and the system time to the received value
   setTime(t);          
   }
   */
  thermoSetup();
}

void loop() {
  runThermo();
  refreshDisplay();
}

/* *********** */
/* * DISPLAY * */
/* *********** */

void refreshDisplay() {
  key = keypad.getKey();
  if (int(key) != 0) {
    lcd.setCursor(0,3);
    lcd.print(key);
  }
  lcd.setCursor(0, 0);
  lcd.print("Thermometer ");
  lcd.print(versionCode);
  lcd.setCursor(0, 1);
  // TIME
  int h = hour();
  int m = minute();
  int s = second();
  if(h < 10)
    lcd.print('0');
  lcd.print(h);
  lcd.print(":");
  if(m < 10)
    lcd.print('0');
  lcd.print(m);
  lcd.print(":");  
  if(s < 10)
    lcd.print('0');
  lcd.print(s);
  // Temperature
  lcd.setCursor(0, 2);
  lcd.print(temperature);
  lcd.print(" C");  
}

/* *********** */
/* ** CLOCK ** */
/* *********** */

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

time_t processSyncMessage() {
  // return the time if a valid sync message is received on the serial port.
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      return pctime; 
    }  
  }
  return 0;
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

