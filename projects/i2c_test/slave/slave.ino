#include <Wire.h>

#define LED_PIN 13

int x;

void setup() {
  Wire.begin(9);                // Start I2C Bus as a Slave (Device Number 9)
  Wire.onReceive(receiveEvent); // register event
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  x = -1;
}

void loop() {
  //If value received is 0 blink LED 1
  if (x != -1) {
    if ( x < 6) {
      for (int i = 0; i < x; i++) {
        digitalWrite(LED_PIN, HIGH);  
        delay (100);
        digitalWrite(LED_PIN, LOW);  
        delay (100);
      }
    } else {
        digitalWrite(LED_PIN, HIGH);  
        delay (500);
        digitalWrite(LED_PIN, LOW);  
    }
    x = -1;
  }
  delay (100);
}

void receiveEvent(int howMany) {
  x = 0;
  x = Wire.read();    // receive byte as an integer
}
