#include <Arduino.h>
#include <MySensor.h>
#include <SPI.h>

#define SENSOR_ID_1 15
#define SENSOR_ID_2 16

#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4

MySensor node;
MyMessage msg1(SENSOR_ID_1, V_TRIPPED);
MyMessage msg2(SENSOR_ID_2, V_TRIPPED);

void setup()
{
  Serial.begin(115200);
  int address;
  while (true) {
    // Getting node setup information
    Serial.println ("Please input node address (1-254)!");
    while (Serial.available() == 0) {
      delay(1);
    }
    address = Serial.parseInt();
    if (address < 1 || address > 254) {
      Serial.print ("Address ");
      Serial.print (address);
      Serial.println (" out of range...");
      delay(500);
    } else {
      break;
    } 
  }
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
      node.send(msg1);
    }
    delay(100); // wait for a second
  } else if (buttonState2 != actual2) {
    buttonState1 = actual1;
    buttonState2 = actual2;
    if (buttonState2 == LOW) {
      node.send(msg2);
    }
    delay(100); // wait for a second
  }

  delay(10);
}
