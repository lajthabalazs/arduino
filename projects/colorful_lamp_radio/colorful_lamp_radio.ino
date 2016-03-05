#include <Arduino.h>
#include <avr/pgmspace.h>

#include <SPI.h>            // The compiler needs to know it in advance
#include <EEPROM.h>

#include <BoardConfig.h>
#include "RF24/printf.h"
#include <SimpleNode.h>

#include "utility/MemoryFree.h"
#include "utility/DebugMessage.h"

#define BLUE_PIN 10
#define GREEN_PIN 5
#define RED_PIN 6

// LED stuff

int maxIndex = 7;
int MAX_LEVEL = 8;
int levels[] = {0,2,4,8,16,32,64,128,255};

long red = 0;
long green = 0;
long blue = 0;
int levelIndex = 0;
int index = 0;

SimpleNode *simpleNode;
uint8_t input[100];
uint8_t inputlen = 0;

unsigned long pingTime;
unsigned long lastStatus = 0;

void pong(uint16_t fromAddress, uint8_t seq, uint16_t delay) {
  printf_P(PSTR("PONG from: %x, seq: %d, time: %u\n"), fromAddress, seq, delay);
}

void receive(uint16_t fromAddress, uint8_t *data, uint8_t length) {
  memcpy((void*) input, (void*) data, length);
  inputlen = length;

  index = 0;
  red = input[0];
  green = input[1];
  blue = input[2];
  levelIndex = input[3];
  if (levelIndex > MAX_LEVEL -1) {
    levelIndex = MAX_LEVEL-1;
  }
}

void connected() {
  Serial.println("Connected");
  Serial.flush();
  input[0] = 0;// Device type
  input[1] = 3;
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

int BACK_BUTTON = A5;
int FORWARD_BUTTON = A4;

long backButtonStart = 0;
long forwardButtonStart = 0;

long lastBackButtonStep = 0;
long lastForwardButtonStep = 0;

boolean backButtonDown = false;
boolean forwardButtonDown = false;

long lastTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Lamp started") ;

  Serial.flush();

  // Setting up LEDs
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BACK_BUTTON, INPUT);
  digitalWrite(BACK_BUTTON, HIGH); // connect internal pull-up

  pinMode(FORWARD_BUTTON, INPUT);
  digitalWrite(FORWARD_BUTTON, HIGH); // connect internal pull-up
  Serial.println("Startng lamp");

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
}

boolean eventProcessed = false;

void loop() {
  // LED stuff
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
  if (!backButtonDown) {
    backButtonStart = t;
    lastBackButtonStep = t;
  }
  if (!forwardButtonDown) {
    forwardButtonStart = t;
    lastForwardButtonStep = t;
  }
  boolean newBackButtonDown = (digitalRead(BACK_BUTTON) == LOW);
  boolean newForwardButtonDown = (digitalRead(FORWARD_BUTTON) == LOW);
  // Button released, check length
  if (!newBackButtonDown && backButtonDown) {
    Serial.print("Back button released, time ");
    Serial.println((t - backButtonStart));
    if ((t - backButtonStart) > 30 && (t-backButtonStart) < 300) {
      if (levelIndex == 0) {
        switchOn();
      } else {
        previousEffect();
      }
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
      if (levelIndex == 0) {
        switchOn();
      } else {
        nextEffect();
      }
    }
  } else if (forwardButtonDown) { // Holding button
    if (t - lastForwardButtonStep > 500) {
      lastForwardButtonStep = t;
      brighten();
    }
  }
  forwardButtonDown = newForwardButtonDown;
  backButtonDown = newBackButtonDown;
  if (lastTime != t) { // Change color only if at least a milli has passed
    setColor(t);
  }

  if (t - lastStatus > 2000) {
    Serial.println("Sending status");
    lastStatus = t;
    // Radio stuff
    yield();
    if (networking->connected) {
      input[0] = 0;// Device type
      input[1] = 3;
      input[2] = red;
      input[3] = green;
      input[4] = blue;
      input[5] = levelIndex;
      
      Serial.print("R:");
      Serial.print(red);
      Serial.print("G:");
      Serial.print(green);
      Serial.print("B:");
      Serial.println(blue);
      Serial.flush();
      
      networking->sendData(0, input, 6);
    }
  }
  lastTime = t;
}

void switchOn() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Switching on");
  levelIndex = 1;
  eventProcessed = true;
}

void dim() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  levelIndex--;
  if (levelIndex < 0) {
    levelIndex = 0;
  }
  Serial.print("Dimming, new level ");
  Serial.println(levelIndex);
  eventProcessed = true;
}

void brighten() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  levelIndex++;
  if (levelIndex > MAX_LEVEL) {
    levelIndex = MAX_LEVEL;
  }
  Serial.print("Brightening, new level ");
  Serial.println(levelIndex);
  eventProcessed = true;
}

int reds[] =   {255,0,0,  255,255,0,  255};
int greens[] = {0,255,0,  255,0,255,  255};
int blues[] =  {0,0,255,  0,255,255,  255};

int pulseIndex = 0;

void nextEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  index++;
  if (index > maxIndex) {
    index = 0;
  }
  Serial.print("Next effect, effect index ");
  Serial.println(index);
  eventProcessed = true;
}

void previousEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  index--;
  if (index < 0) {
    index = maxIndex;
  }
  Serial.print("Previous effect, effect index ");
  Serial.println(index);
  eventProcessed = true;
}

int lastIntensity = 0;

void setColor(long time) {
  if (index == maxIndex) {
    // Determine color based on time
    long intensity = (time / 4) % 512;
    if (intensity == lastIntensity) {
      return;
    }
    lastIntensity = intensity;
    if (intensity == 0) {
      pulseIndex++;
      Serial.print("Next pulse color, index ");
      if (pulseIndex == maxIndex) {
        pulseIndex = 0;
      }
      Serial.println(pulseIndex);
    }
    if (intensity > 255) {
      intensity = 511 - intensity;
    }
    red = (reds[pulseIndex] * intensity) / 255;
    green = (greens[pulseIndex] * intensity) / 255;
    blue = (blues[pulseIndex] * intensity) / 255;
  } else {
    red = reds[index];
    green = greens[index];
    blue = blues[index];
  }
  analogWrite(RED_PIN, (int)(red * levels[levelIndex] / 255));
  analogWrite(GREEN_PIN, (int)(green * levels[levelIndex] / 255));
  analogWrite(BLUE_PIN, (int)(blue * levels[levelIndex] / 255));
}
