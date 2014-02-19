#include <LiquidCrystal.h>



LiquidCrystal lcd(2, 12, 3, 4, 5, 6, 7, 8, 9, 10, 11);

void setup() {
  lcd.begin(20,4);
  lcd.print("Hello, world!ááá");
  
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
}
