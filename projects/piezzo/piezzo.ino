#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// these constants won't change:
const int sPin[] = {A0,A1,A2,A3};
const int xPos[] = {0,11, 0, 11};
const int yPos[] = {0,0, 1, 1};
 
// these variables will change:
int s[] = {0,0,0,0};
int m[] = {0,0,0,0};
int mm[] = {0,0,0,0};
int len[] = {1,1,1,1};

LiquidCrystal_I2C lcd(0x27, 16, 2);
//set up
void setup() {
  Serial.begin(9600); // use the serial port
  lcd.begin();   // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight();
}
 
//Main program
int first = 0;
int second = 0;

void loop() {
  first++;
  if (first++ > 100) {
    first  = 0;
    if (second ++ > 5) {
      second = 0;
      for (int i = 0; i < sizeof(s) / 2; i++) {
        mm[i] = mm[i] / 12;
        if (mm[i] >= 10) {
          len[i] = 2;
        } else {
          len[i] = 1;
        }
        lcd.setCursor(xPos[i],yPos[i]);
        lcd.print("  ");
        lcd.setCursor(xPos[i] + 2 - len[i],yPos[i]);
        lcd.print(mm[i]);
        mm[i] = 0;
      }
    }
    for (int i = 0; i < sizeof(s) / 2; i++) {
      m[i] = m[i] / 12;
      if (m[i] >= 10) {
        len[i] = 2;
      } else {
        len[i] = 1;
      }
      lcd.setCursor(xPos[i] + 3,yPos[i]);
      lcd.print("  ");
      lcd.setCursor(xPos[i] + 5 - len[i],yPos[i]);
      lcd.print(m[i]);
      m[i] = 0;
      s[i] = 0;
    }
  }
  for (int i = 0; i < sizeof(s) / 2; i++) {
    m[i] = max(m[i], s[i]);
    mm[i] = max(mm[i],s[i]);
  }

  for (int i  = 0; i < sizeof(sPin)/2; i++){
    s[i] = analogRead(sPin[i]);
  }

 delay(1);
}
