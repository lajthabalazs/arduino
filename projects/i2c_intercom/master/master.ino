#include <Wire.h>

#define LED_PIN 13
byte x = 0;

void setup()
{
  Wire.begin(); // Start I2C Bus as Master
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
void loop()
{  
  x++;
  if (x > 5) x=0;
  for (int i = 0; i < x; i++) {
    digitalWrite(LED_PIN, LOW);  
    delay (100);
    digitalWrite(LED_PIN, HIGH);  
    delay (100);
  }
  digitalWrite(LED_PIN, LOW);  
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(x);              // sends x
  Wire.endTransmission();    // stop transmitting
  delay(3000);
}
