#include <Servo.h>

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <SPI.h>            // The compiler needs to know it in advance
#include <EEPROM.h>

//#define SS_BOARD_V1
//#define SS_BOARD_V3       // Do it in the general setup

#define WHEEL_PIN 10
#define ENGINE_PIN1 5
#define ENGINE_PIN2 6

#include <BoardConfig.h>
#include "RF24/printf.h"
#include <SimpleNode.h>

#include "utility/MemoryFree.h"
#include "utility/DebugMessage.h"

SimpleNode *simpleNode;
uint8_t input[100];
uint8_t inputlen = 0;

unsigned long pingTime;

Servo wheel;

int wheelstate = 0;
int enginestate = 0;

int enginespeeds[] = { 0, 150, 180, 200, 220, 240, 255 };

void pong(uint16_t fromAddress, uint8_t seq, uint16_t delay) {
  printf_P(PSTR("PONG from: %x, seq: %d, time: %u\n"), fromAddress, seq, delay);
}

void receive(uint16_t fromAddress, uint8_t *data, uint8_t length) {
  memcpy((void*) input, (void*) data, length);
  inputlen = length;

  Serial.println("Command received");
  Serial.println((char*) input);
  
  char command = (char) input[0];
  int cvalue = input[1] - '0';
  
  switch (command) {
    case 'L':
      wheel.write(90 + cvalue * 5);
      wheelstate = cvalue;
      break;
    case 'R':
      wheel.write(90 - cvalue * 5);
      wheelstate = -cvalue;
      break;
    case 'F':
      if (cvalue < 5) {
        analogWrite(ENGINE_PIN1, enginespeeds[cvalue]);\
      } else {
        digitalWrite(ENGINE_PIN1, 1);
      }
      digitalWrite(ENGINE_PIN2, 0);
      enginestate = cvalue;
      break;
    case 'B':
      digitalWrite(ENGINE_PIN1, 0);
      if (cvalue < 5) {
        analogWrite(ENGINE_PIN2, enginespeeds[cvalue]);
      } else {
        digitalWrite(ENGINE_PIN2, 1);
      }
      enginestate = -cvalue;
      break;
  }
}

void connected() {
  Serial.println("Connected");
  Serial.flush();
  networking->sendData(0, (uint8_t*) "CAR_READY", 10);
}

void disconnected() {
  Serial.println("Disconnected");
  Serial.flush();

  // Stop Engines
  digitalWrite(ENGINE_PIN1, 0);
  digitalWrite(ENGINE_PIN2, 0);
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Node started.");
  Serial.flush();

  pinMode(WHEEL_PIN, OUTPUT);
  pinMode(ENGINE_PIN1, OUTPUT);
  pinMode(ENGINE_PIN2, OUTPUT);

  wheel.attach(WHEEL_PIN);
  wheel.write(90);

  printf_begin();

  simpleNode = new SimpleNode();
  networking->setPongReceiver(&pong);
  networking->setDataReceiver(&receive);
  networking->setConnectReceiver(&connected);
  networking->setDisconnectReceiver(&disconnected);

  #ifdef SS_WITH_LEDS
    pinMode(SS_LED_RED, OUTPUT);
    pinMode(SS_LED_GREEN, OUTPUT);
    radio->setLEDs(SS_LED_RED, SS_LED_GREEN);
  #endif // SS_WITH_LEDS

  strcpy((char*) input, "Hello");
  inputlen = strlen((char*) input);
}

void loop()
{

  yield();

  sleep(2000);
  if (networking->connected) {

    int c = 0;
    input[c++] = 'W';
    if (wheelstate<0) input[c++] = '-';
    input[c++] = abs(wheelstate) + '0';
    input[c++] = 'E';
    if (enginestate<0) input[c++] = '-';
    input[c++] = abs(enginestate) + '0';
    input[c++] = 0;

    Serial.println((char*) input);
    Serial.flush();
    
    networking->sendData(0, input, c);
  }

  yield();
  //sleep(10);
  //debug_println_F("Send broadcast ping.");
  //networking->ping();
}
