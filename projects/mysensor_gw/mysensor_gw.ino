#include <SPI.h>  
#include <MySensor.h>  
#include <MyGateway.h>  
#include <stdarg.h>

#define INCLUSION_MODE_TIME 1 // Number of minutes inclusion mode is enabled
#define INCLUSION_MODE_PIN 3 // Digital pin used for inclusion mode button

MyGateway gw(DEFAULT_CE_PIN, DEFAULT_CS_PIN, INCLUSION_MODE_TIME, INCLUSION_MODE_PIN,  6, 5, 4);

char inputString[MAX_RECEIVE_LENGTH] = "";    // A string to hold incoming commands from serial/ethernet interface
int inputPos = 0;
boolean commandComplete = false;  // whether the string is complete

void setup()  
{ 
  gw.begin();
}

void loop()  
{ 
  gw.processRadioMessage();   
  if (commandComplete) {
    // A command wass issued from serial interface
    // We will now try to send it to the actuator
    gw.parseAndSend(inputString);
    commandComplete = false;  
    inputPos = 0;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inputPos<MAX_RECEIVE_LENGTH-1 && !commandComplete) { 
      if (inChar == '\n') {
        inputString[inputPos] = 0;
        commandComplete = true;
      } else {
        // add it to the inputString:
        inputString[inputPos] = inChar;
        inputPos++;
      }
    } else {
       // Incoming message too long. Throw away 
        inputPos = 0;
    }
  } 
}
