#include <Arduino.h>
#include <MySensor.h>
#include <SPI.h>

#define INCLUSION_MODE_TIME 1 // Number of minutes inclusion mode is enabled
#define INCLUSION_MODE_PIN 3 // Digital pin used for inclusion mode button



#define SENSOR_ID_1 15
#define SENSOR_ID_2 16
#define OPEN 1
#define CLOSE 0
#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4

MySensor node;
MyMessage msg1(SENSOR_ID_1, V_TRIPPED);
MyMessage msg2(SENSOR_ID_2, V_TRIPPED);

int buttonState1 = HIGH;
int buttonState2 = HIGH;

void setup()
{
	node.begin();
	node.present(SENSOR_ID, S_DOOR);
        pinMode(BUTTON_PIN_1, INPUT);
        digitalWrite(BUTTON_PIN_1, HIGH); // connect internal pull-up
        pinMode(BUTTON_PIN_2, INPUT);
        digitalWrite(BUTTON_PIN_2, HIGH); // connect internal pull-up
}

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
