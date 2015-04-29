#include <SPI.h>  
#include <MySensor.h>  
#include <MyGateway.h>  
#include <stdarg.h>

#define INCLUSION_MODE_TIME 1 // Number of minutes inclusion mode is enabled
#define INCLUSION_MODE_PIN 3 // Digital pin used for inclusion mode button

int ledPins[] = {4,5,6,7,8,13};
int length = 6;


MyGateway gw(9, 10, INCLUSION_MODE_TIME, INCLUSION_MODE_PIN,  6, 5, 4);

char inputString[MAX_RECEIVE_LENGTH] = "";    // A string to hold incoming commands from serial/ethernet interface
int inputPos = 0;
boolean commandComplete = false;  // whether the string is complete

void setup()  
{ 
  for (int i = 0; i < length; i++) {
  pinMode(ledPins[i], OUTPUT);
  digitalWrite(ledPins[i], HIGH);
}
  gw.begin();
}

void loop()  
{ 
  int value = gw.processRadioMessage();
  if (value != -1) {
    Serial.print("Sensor: ");
    Serial.println(value);
    int out = LOW;
    int index = value % 100;
    if ( value > 150) {
      out = HIGH;
    }
    digitalWrite(ledPins[index], out);
  }
}
