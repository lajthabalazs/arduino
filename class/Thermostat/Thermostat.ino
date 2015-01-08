#include <OneWire.h>
#include <DallasTemperature.h>

int upButtonPin = 4;
int downButtonPin = 7;
int ledPin = 13;

boolean upPressed = false;
boolean downPressed = false;
int target = 20;

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  pinMode(upButtonPin, INPUT);
  digitalWrite(upButtonPin, HIGH);
  pinMode(downButtonPin, INPUT);
  digitalWrite(downButtonPin, HIGH);
  initDisplay();
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures();
  int temp = (int)sensors.getTempCByIndex(0);
  for (int i =0; i < 100; i++) {
  if(digitalRead(upButtonPin) == LOW) {
    if (upPressed == false) {
      target = target + 1;
    }
    upPressed = true;
  } else {
    upPressed = false;
  }
  if(digitalRead(downButtonPin) == LOW) {
    if (downPressed == false) {
      target = target - 1;
    }
    downPressed = true;
  } else {
    downPressed = false;
  }
    displayValue(target, temp);
    if (target > temp) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}

int clock = 11; //SCK
int latch = 12; //RCK 
int data = 10;  //DIO

void initDisplay() {
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
}

byte segment[] ={
                B11000000, // 0
                B11111001, // 1
                B10100100, // 2
                B10110000, // 3
                B10011001, // 4
                B10010010, // 5
                B10000010, // 6
                B11111000, // 7
                B10000000, // 8
                B10010000, // 9
                B01111111, // .
                B11111111};// display nothing

byte digit[] ={ B00000001, // right segment
                B00000010,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01000000,
                B10000000}; // left segment


void displayValue(int target, int temp) {
  if (target < 0) {
    target = 0;
  }
  int value = target * 100 + temp;
  int digits[] = {value % 10, (value / 10) % 10, (value / 100) % 10, (value / 1000) % 10};

  long start = millis();
  long current = start;
  while (current < start + 50 && current >= start) {
    current = millis();
    for (int i = 0; i < 10; i++) {
      for (int d = 0; d < 4; d++) {
        digitalWrite(latch,LOW);
        // select the fourth segment from left
        shiftOut(data,clock,MSBFIRST,segment[digits[d]]);
        // show the digit "5" ( the array starts with 0 to count!)
        shiftOut(data,clock,MSBFIRST,digit[d]);  
        digitalWrite(latch,HIGH);
      }
    }
  }
}
