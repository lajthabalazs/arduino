#include <LedControl.h>

int upButtonPin = 4;
int downButtonPin = 7;
int ledPin = 13;

boolean upPressed = false;
boolean downPressed = false;
int value = 0;

void setup(void)
{
  Serial.begin(9600);
  pinMode(upButtonPin, INPUT);
  digitalWrite(upButtonPin, HIGH);
  pinMode(downButtonPin, INPUT);
  digitalWrite(downButtonPin, HIGH);
  initDisplay();
}

void loop(void)
{
  if(digitalRead(upButtonPin) == LOW) {
    if (upPressed == false) {
      // TODO increment target
    }
    upPressed = true;
  } else {
    upPressed = false;
  }
  if(digitalRead(downButtonPin) == LOW) {
    if (downPressed == false) {
      // TODO decrement target
    }
    downPressed = true;
  } else {
    downPressed = false;
  }
  // TODO display value
  delay(1);
}

//                        din,clk,cs 
LedControl lc1=LedControl(12, 10, 11,1); 


void initDisplay() {
 for(int index=0;index<lc1.getDeviceCount();index++) {
      lc1.shutdown(index,false);
 }
}

int xTen[] = {5,5,5,4,4,4};
int yTen[] = {7,6,5,7,6,5};

int xOne[] = {7,7,7,7,7,6,6,6,6,6};
int yOne[] = {0,1,2,3,4,0,1,2,3,4};

void displayValue(int value) {
  if (value < 0) {
    value = 0;
  }
  // Displays values between 0 and 6
    }
    for (int tens = 1; tens < 6; tens++) {
      lc1.setLed(0,xTen[tens - 1], yTen[tens - 1], value >= tens * 10);
    }
    for (int ones = 1; ones < 10; ones++) {
      lc1.setLed(0,xOne[ones - 1], yOne[ones - 1], value % 10 >= ones);
    }
  }
}
