#include <OneWire.h>
#include <DallasTemperature.h>

#include <LedControl.h>

int upButtonPin = 4;
int downButtonPin = 7;
int ledPin = 13;

boolean upPressed = false;
boolean downPressed = false;
int value = 20;

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
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
    //Serial.println(value);
    delay(10);
    if(digitalRead(upButtonPin) == LOW) {
      if (upPressed == false) {
        value = value + 1;
      }
      upPressed = true;
    } else {
      upPressed = false;
    }
    if(digitalRead(downButtonPin) == LOW) {
      if (downPressed == false) {
        value = value - 1;
      }
      downPressed = true;
    } else {
      downPressed = false;
    }
    displayValue(value, temp);
    if (value > temp) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}
//                        din,clk,cs 
LedControl lc1=LedControl(12, 10, 11,1); 


void initDisplay() {
 for(int index=0;index<lc1.getDeviceCount();index++) {
      lc1.shutdown(index,false);
 }
}

int xTen[] = {1,1,1,0,0,0};
int yTen[] = {7,6,5,7,6,5};

int xOne[] = {3,3,3,3,3,2,2,2,2,2};
int yOne[] = {0,1,2,3,4,0,1,2,3,4};

void displayValue(int target, int temp) {
  if (target < 0) {
    target = 0;
  }
  if (temp < 0) {
    temp = 0;
  }
  // Displays values between 0 and 6
  int xOffset = 0;
  int data = 0;
  for (int i = 0; i < 2; i++) {
    if (i == 0) {
      data = target;
    } else {
      data = temp;
      xOffset = 4;
    }
    for (int tens = 1; tens < 6; tens++) {
      lc1.setLed(0,xTen[tens - 1] + xOffset, yTen[tens - 1], data >= tens * 10);
    }
    for (int ones = 1; ones < 10; ones++) {
      lc1.setLed(0,xOne[ones - 1] + xOffset, yOne[ones - 1], data % 10 >= ones);
    }
  }
}
