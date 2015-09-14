#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BACKLIGHT_PIN     12

void setup()
{
  // Switch on the backlight
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  
  lcd.begin();               // initialize the lcd 
  lcd.home();
  lcd.noBlink();
  digitalWrite ( 13, HIGH );
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("A");
  lcd.setCursor(8,0);
  lcd.print("A");
  lcd.setCursor(0,1);
  lcd.print("A");
  lcd.setCursor(8,1);
  lcd.print("A");
  digitalWrite ( 13, HIGH );
  delay(200);
  digitalWrite ( 13, LOW );
  delay(200);
}
