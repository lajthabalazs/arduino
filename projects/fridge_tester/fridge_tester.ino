#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  lcd.begin();
  lcd.backlight();
}

void loop() {
 // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  for (int i = 0; i < 1; i++) {
    Serial.print("Temperature for Device ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(sensors.getTempCByIndex(i)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  }
  lcd.setCursor(0,0);
  lcd.print(sensors.getTempCByIndex(0));
  delay(1000);
}
