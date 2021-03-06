#include <Time.h>
#include <DS1307RTC.h>

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <OneWire.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define versionCode "v0.7.5"

enum State { Display, SetTime };
State state = Display;

/* ******************* */
/* ****** SET TIME *** */
/* ******************* */
int cursorPosition = 0;
long cursorBlink = 0;
boolean cursorBlinkState = true;
int yearFirst = 0;
int yearSecond = 0;
int yearThird = 0;
int yearFourth = 0;
int monthFirst = 0;
int monthSecond = 0;
int dayFirst = 0;
int daySecond = 0;
int hoursFirst = 0;
int hoursSecond = 0;
int minutesFirst = 0;
int minutesSecond = 0;

OneWire thermometer(7); // ds18s20 thermometer
LiquidCrystal lcd(13, 12, 3, 8, 9, 10, 11);
byte present = 0;

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
char lastKey = 0;

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
  thermoSetup();
}

void loop() {
  // Get input from keypad
  key = keypad.getKey();
  if (int(key) != 0) {
    // Cache input until processed
    lastKey = key;
  }
  runThermo();
  if (cursorBlink < millis()) {
    cursorBlink = millis() + 1000;
    cursorBlinkState = !cursorBlinkState;
  }
  refreshDisplay();
}

/* *********** */
/* * DISPLAY * */
/* *********** */

void refreshDisplay() {
  lcd.setCursor(0,3);
  long secs = millis()/1000;
  lcd.print("Uptime ");
  lcd.print(secs / 3600 / 24);
  lcd.print('d');
  int h = secs / 3600 % 24;
  if (h < 10) {
    lcd.print('0');
  }
  lcd.print(h);
  lcd.print('h');
  int m = secs /60 % 60;
  if (m < 10) {
    lcd.print('0');
  }
  lcd.print(m);
  lcd.print("m");
  
  updateState();
  lcd.setCursor(0, 0);
  lcd.print("Thermometer ");
  lcd.print(versionCode);
  // Temperature
  lcd.setCursor(0, 2);
  lcd.print(temperature);
  lcd.print(" C");
  if (state == Display) {
    displayTime();
  } else if (state == SetTime){
    displaySetTime();
  }
}

void updateState() {
  if (state == Display && lastKey == 'A') {
    state = SetTime;
    lastKey = 0;
    yearFirst = year()/1000;
    yearSecond = year()/100 % 10;
    yearThird = year()/10 % 10;
    yearFourth = year() % 10;
    monthFirst = month()/10;
    monthSecond = month() % 10;
    dayFirst = day()/10;
    daySecond = day()%10;
    hoursFirst = hour()/10;
    hoursSecond = hour()%10;
    minutesFirst = minute()/10;
    minutesSecond = minute()%10;
  }
  if (state == SetTime) {
    if (lastKey != 0){
      Serial.print("Last Key ");
      Serial.println(lastKey);
    }
    if (lastKey == 'D') {
      state = Display;
      lastKey = 0;
      resetEdit();
      return;
    }
    int inputValue = lastKey - '0';
    if (inputValue < 10 && inputValue > -1) {
      Serial.print("Got a number ");
      Serial.println(inputValue);
      switch(cursorPosition){
        // YEAR
        case 0:
          yearFirst = inputValue;
          cursorPosition++;
          break;
        case 1:
          yearSecond = inputValue;
          cursorPosition++;
          break;
        case 2:
          yearThird = inputValue;
          cursorPosition++;
          break;
        case 3:
          yearFourth = inputValue;
          cursorPosition++;
          break;
        // MONTH
        case 4:
          if (inputValue < 2) {
            monthFirst = inputValue;
            cursorPosition++;
          }
          break;
        case 5:
          if ((hoursFirst < 1) || inputValue < 3) {
            monthSecond = inputValue;
            cursorPosition++;
          }
          break;
        // DAY
        case 6:
          if (inputValue < 4) {
            dayFirst = inputValue;
            cursorPosition++;
          }
          break;
        case 7:
          if ((hoursFirst < 3) || inputValue < 2) {
            daySecond = inputValue;
            cursorPosition++;
          }
          break;
        
        case 8:
          if (inputValue < 3) {
            hoursFirst = inputValue;
            cursorPosition++;
          }
          break;
        case 9:
          if ((hoursFirst < 2) || inputValue < 4) {
            hoursSecond = inputValue;
            cursorPosition++;
          }
          break;
        case 10:
          if (inputValue < 6) {
            minutesFirst = inputValue;
            cursorPosition++;
          }
          break;
        case 11:
          minutesSecond = inputValue;
          setTime(yearFirst * 1000 + yearSecond * 100 + yearThird*10 + yearFourth, monthFirst * 10 + monthSecond, dayFirst * 10 + daySecond, hoursFirst * 10 + hoursSecond, minutesFirst * 10 + minutesSecond);
          resetEdit();
          break;
      }
      // Consume key
    } else if (lastKey == '*') {
      Serial.print("Got a star ");
      if (cursorPosition > 0) {
        cursorPosition--;
      }
    } else if (lastKey == '#') {
      Serial.print("Got a hashmark ");
      if (cursorPosition < 11) {
        cursorPosition++;
      } else {
        setTime(yearFirst * 1000 + yearSecond * 100 + yearThird*10 + yearFourth, monthFirst * 10 + monthSecond, dayFirst * 10 + daySecond, hoursFirst * 10 + hoursSecond, minutesFirst * 10 + minutesSecond);
        resetEdit();
      }
    }
    lastKey = 0;
  }
}

void resetEdit() {
  yearFirst = 0;
  yearSecond = 0;
  yearThird = 0;
  yearFourth = 0;
  monthFirst = 0;
  monthSecond = 0;
  dayFirst = 0;
  daySecond = 0;
  hoursFirst = 0;
  hoursSecond = 0;
  minutesFirst = 0;
  minutesSecond = 0;
  state = Display;
  cursorPosition = 0;
}

void displayTime() {
  lcd.setCursor(0, 1);
  int y = year();
  int mo = month();
  int d = day();
  int h = hour();
  int m = minute();
  int s = second();
  lcd.print(y);
  if (y < 1000) {
    lcd.print('0');
    if (y < 100) {
      lcd.print('0');
      if (y < 10) {
        lcd.print('0');
      }
    }
  }
  lcd.print('/');
  if(mo < 10) {
    lcd.print('0');
  }
  lcd.print(mo);
  lcd.print('/');
  if(d < 10) {
    lcd.print('0');
  }
  lcd.print(d);
  lcd.print(' ');

  if(h < 10) {
    lcd.print('0');
  }
  lcd.print(h);
  lcd.print(":");
  if(m < 10) {
    lcd.print('0');
  }
  lcd.print(m);
  lcd.print(":");  
  if(s < 10) {
    lcd.print('0');
  }
  lcd.print(s);
}

void displaySetTime() {
  lcd.setCursor(0, 1);
  if (cursorBlinkState && cursorPosition == 0) { 
    lcd.print("_");
  } else {
    lcd.print(yearFirst);
  }
  if (cursorBlinkState && cursorPosition == 1) { 
    lcd.print("_");
  } else {
    lcd.print(yearSecond);
  }
  if (cursorBlinkState && cursorPosition == 2) { 
    lcd.print("_");
  } else {
    lcd.print(yearThird);
  }
  if (cursorBlinkState && cursorPosition == 3) { 
    lcd.print("_");
  } else {
    lcd.print(yearFourth);
  }
  lcd.print("/");  
  if (cursorBlinkState && cursorPosition == 4) { 
    lcd.print("_");
  } else {
    lcd.print(monthFirst);
  }
  if (cursorBlinkState && cursorPosition == 5) { 
    lcd.print("_");
  } else {
    lcd.print(monthSecond);
  }
  lcd.print("/");  
  if (cursorBlinkState && cursorPosition == 6) { 
    lcd.print("_");
  } else {
    lcd.print(dayFirst);
  }
  if (cursorBlinkState && cursorPosition == 7) { 
    lcd.print("_");
  } else {
    lcd.print(daySecond);
  }
  lcd.print(" ");  
  if (cursorBlinkState && cursorPosition == 8) { 
    lcd.print("_");
  } else {
    lcd.print(hoursFirst);
  }
  if (cursorBlinkState && cursorPosition == 9) { 
    lcd.print("_");
  } else {
    lcd.print(hoursSecond);
  }
  lcd.print(":");
  if (cursorBlinkState && cursorPosition == 10) { 
    lcd.print("_");
  } else {
    lcd.print(minutesFirst);
  }
  if (cursorBlinkState && cursorPosition == 11) { 
    lcd.print("_");
  } else {
    lcd.print(minutesSecond);
  }
  lcd.print("   ");
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

void setTime(int year, int month, int day, int hours, int minutes) {
  setTime(hours, minutes, 0, day, month, year);
  RTC.set(now());
}

/* *********** */
/* THERMOMETER */
/* *********** */

byte data[12];
byte addr[8];
float temperature = 0;

long nextThermoEvent = 0;
byte thermoState = 0; // 0 before setup, 1 before read, 2 processing

void runThermo() {
  if (nextThermoEvent < millis()) {
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

