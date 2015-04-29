#include <Arduino.h>
#include <MySensor.h>
#include <SPI.h>

#define SENSOR_ID_1 102
#define SENSOR_ID_2 202
// 102 > green board

#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4

MySensor node;
MyMessage msg1(SENSOR_ID_1, V_TRIPPED);
MyMessage msg2(SENSOR_ID_2, V_TRIPPED);
int led = 0;
long time = 0;

void setup()
{
  Serial.begin(115200);
  int address = SENSOR_ID_1;
  Serial.print ("Address received: ");
  Serial.println (address);
  node.setAddress(address);
  node.begin();
  node.present(SENSOR_ID_1, S_DOOR);
  node.present(SENSOR_ID_2, S_DOOR);


  pinMode(BUTTON_PIN_1, INPUT);
  digitalWrite(BUTTON_PIN_1, HIGH); // connect internal pull-up

  pinMode(BUTTON_PIN_2, INPUT);
  digitalWrite(BUTTON_PIN_2, HIGH); // connect internal pull-up
  time = millis();
}

int buttonState1 = HIGH;
int buttonState2 = HIGH;

void loop()
{
  int actual1 = digitalRead(BUTTON_PIN_1);
  int actual2 = digitalRead(BUTTON_PIN_2);
  if (buttonState1 != actual1) {
    buttonState1 = actual1;
    buttonState2 = actual2;
    if (buttonState1 == LOW) {
      Serial.println("Button 1 pressed");
      node.send(msg1);
    }
    delay(100); // wait for a moment
  } else if (buttonState2 != actual2) {
    buttonState1 = actual1;
    buttonState2 = actual2;
    if (buttonState2 == LOW) {
      Serial.println("Button 2 pressed");
      node.send(msg2);
    }
    delay(100); // wait for a moment
  }
  delay(10);
}
