#include <Arduino.h>
#include <avr/pgmspace.h>

#include <SPI.h>            // The compiler needs to know it in advance
#include <EEPROM.h>

#define DEVICE_LOW 17
#define DEVICE_HIGH 0

#define BLUE_PIN 10
#define GREEN_PIN 5
#define RED_PIN 6

#include <BoardConfig.h>
#include "RF24/printf.h"
#include <SimpleNode.h>

#include "utility/MemoryFree.h"
#include "utility/DebugMessage.h"

SimpleNode *simpleNode;
uint8_t input[100];
uint8_t inputlen = 0;

unsigned long pingTime;

int r = 0;
int g = 0;
int b = 0;

// BUTTON HANDLING

int BACK_BUTTON = A5;
int FORWARD_BUTTON = A4;
long backButtonStart = 0;
long forwardButtonStart = 0;
long lastBackButtonStep = 0;
long lastForwardButtonStep = 0;
boolean backButtonDown = false;
boolean forwardButtonDown = false;
long lastTime = 0;


void pong(uint16_t fromAddress, uint8_t seq, uint16_t delay) {
  printf_P(PSTR("PONG from: %x, seq: %d, time: %u\n"), fromAddress, seq, delay);
}

void receive(uint16_t fromAddress, uint8_t *data, uint8_t length) {
  memcpy((void*) input, (void*) data, length);
  inputlen = length;

  r = input[0];
  analogWrite(RED_PIN, r);
  g = input[1];
  analogWrite(GREEN_PIN, g);
  b = input[2];
  analogWrite(BLUE_PIN, b);

}

void connected() {
  Serial.println("Connected");
  Serial.flush();
  input[0] = DEVICE_LOW;// Device type
  input[1] = DEVICE_HIGH;
  networking->sendData(0, input, 2);
}

void disconnected() {
  Serial.println("Disconnected");
  Serial.flush();

  // Stop Engines
  digitalWrite(RED_PIN, 0);
  digitalWrite(GREEN_PIN, 0);
  digitalWrite(BLUE_PIN, 0);
}


void setup()
{
 
  Serial.begin(115200);
  Serial.println("Node started") ;

  Serial.flush();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BACK_BUTTON, INPUT);
  digitalWrite(BACK_BUTTON, HIGH); // connect internal pull-up

  pinMode(FORWARD_BUTTON, INPUT);
  digitalWrite(FORWARD_BUTTON, HIGH); // connect internal pull-up
  

  printf_begin();

  NodeConfig nc;
  nc.address = 0x4c34; //"L4"
  nc.netprefix = 0x424D45L;
  nc.radioCE = SS_CE_PIN;
  nc.radioCS = SS_CS_PIN;
  nc.radioChannel = 0;

  simpleNode = new SimpleNode(&nc);
  //saveNodeConfig(&nc);
  //simpleNode = new SimpleNode();
  networking->setPongReceiver(&pong);
  networking->setDataReceiver(&receive);
  networking->setConnectReceiver(&connected);
  networking->setDisconnectReceiver(&disconnected);

  #ifdef SS_WITH_LEDS
    pinMode(SS_LED_RED, OUTPUT);
    pinMode(SS_LED_GREEN, OUTPUT);
    radio->setLEDs(SS_LED_RED, SS_LED_GREEN);
  #endif // SS_WITH_LEDS

}

long lastStatusMessage = 0;
boolean eventProcessed = false;

void loop()
{
  eventProcessed = false;
  long t = millis();
  if (lastTime == t) {
     delay (1);
     return;
  }
  if ((lastTime == 0) || (lastTime > t)) {
    Serial.print("Reseting timers, last: ");
    Serial.print (lastTime);
    Serial.print (", time: ");
    Serial.println (t);
    lastTime = t;
    lastBackButtonStep = t;
    lastForwardButtonStep = t;
    backButtonStart = t;
    forwardButtonStart = t;
    delay(10);
    return;
  }
  updateButtonState(t);
  if (t - lastStatusMessage > 2000) {
    yield();
    if (networking->connected) {
      input[0] = DEVICE_HIGH;// Device type
      input[1] = DEVICE_LOW;
      input[2] = r;
      input[3] = g;
      input[4] = b;
      input[5] = 0;
      
      Serial.print("R:");
      Serial.print(r);
      Serial.print("G:");
      Serial.print(g);
      Serial.print("B:");
      Serial.println(b);
      Serial.flush();
      
      networking->sendData(0, input, 6);
    }
    lastStatusMessage = t;
  }
  lastTime = t;
}

void updateButtonState(long t) {
  if (!backButtonDown) {
    backButtonStart = t;
    lastBackButtonStep = t;
  } else {
    Serial.print("Back button down");
  }
  if (!forwardButtonDown) {
    forwardButtonStart = t;
    lastForwardButtonStep = t;
  } else {
    Serial.print("FW button down");
  }
  boolean newBackButtonDown = (digitalRead(BACK_BUTTON) == LOW);
  boolean newForwardButtonDown = (digitalRead(FORWARD_BUTTON) == LOW);
  // Button released, check length
  if (!newBackButtonDown && backButtonDown) {
    Serial.print("Back button released, time ");
    Serial.println((t - backButtonStart));
    if ((t - backButtonStart) > 30 && (t-backButtonStart) < 300) {
      previousEffect();
    }
  } else if (backButtonDown) { // Holding button
    if (t - lastBackButtonStep > 500) {
      lastBackButtonStep = t;
      dim();
    }
  }
  if (!newForwardButtonDown && forwardButtonDown) {
    Serial.print("Forward button released, time ");
    Serial.println((t - forwardButtonStart));
    if ((t - forwardButtonStart) > 30 && (t-forwardButtonStart) < 300) {
      nextEffect();
    }
  } else if (forwardButtonDown) { // Holding button
    if (t - lastForwardButtonStep > 500) {
      lastForwardButtonStep = t;
      brighten();
    }
  }
  forwardButtonDown = newForwardButtonDown;
  backButtonDown = newBackButtonDown;
}

void switchOn() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Switching on");
  if (networking->connected) {
    input[0] = DEVICE_HIGH;// Device type
    input[1] = DEVICE_LOW;
    input[2] = r;
    input[3] = g;
    input[4] = b;
    input[5] = 1;
    
    Serial.print("R:");
    Serial.print(r);
    Serial.print("G:");
    Serial.print(g);
    Serial.print("B:");
    Serial.println(b);
    Serial.flush();
    
    networking->sendData(0, input, 6);
  }
  eventProcessed = true;
}

void dim() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Dimming");
  if (networking->connected) {
    input[0] = DEVICE_HIGH;// Device type
    input[1] = DEVICE_LOW;
    input[2] = r;
    input[3] = g;
    input[4] = b;
    input[5] = 2;
    
    Serial.print("R:");
    Serial.print(r);
    Serial.print("G:");
    Serial.print(g);
    Serial.print("B:");
    Serial.println(b);
    Serial.flush();
    
    networking->sendData(0, input, 6);
  }
  eventProcessed = true;
}

void brighten() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Brightening");
  if (networking->connected) {
    input[0] = DEVICE_HIGH;// Device type
    input[1] = DEVICE_LOW;
    input[2] = r;
    input[3] = g;
    input[4] = b;
    input[5] = 3;
    
    Serial.print("R:");
    Serial.print(r);
    Serial.print("G:");
    Serial.print(g);
    Serial.print("B:");
    Serial.println(b);
    Serial.flush();
    
    networking->sendData(0, input, 6);
  }
  eventProcessed = true;
}

void nextEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Next effect");
  if (networking->connected) {
    input[0] = DEVICE_HIGH;// Device type
    input[1] = DEVICE_LOW;
    input[2] = r;
    input[3] = g;
    input[4] = b;
    input[5] = 4;
    
    Serial.print("R:");
    Serial.print(r);
    Serial.print("G:");
    Serial.print(g);
    Serial.print("B:");
    Serial.println(b);
    Serial.flush();
    
    networking->sendData(0, input, 6);
  }
  eventProcessed = true;
}

void previousEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Previous effect");
  if (networking->connected) {
    input[0] = DEVICE_HIGH;// Device type
    input[1] = DEVICE_LOW;
    input[2] = r;
    input[3] = g;
    input[4] = b;
    input[5] = 5;
    
    Serial.print("R:");
    Serial.print(r);
    Serial.print("G:");
    Serial.print(g);
    Serial.print("B:");
    Serial.println(b);
    Serial.flush();
    
    networking->sendData(0, input, 6);
  }
  eventProcessed = true;
}
