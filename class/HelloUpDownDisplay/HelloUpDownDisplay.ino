int upButtonPin = 4;
int downButtonPin = 7;
const int clock = 11; //SCK
const int latch = 12; //RCK 
const int data = 10;  //DIO

boolean upPressed = false;
boolean downPressed = false;
int value = 0;

void setup()
{
  pinMode(upButtonPin, INPUT);
  digitalWrite(upButtonPin, HIGH);
  pinMode(downButtonPin, INPUT);
  digitalWrite(downButtonPin, HIGH);
  initDisplay();
}

void loop()
{
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
   int v0 = value % 10;
   int v1 = (value / 10) % 10;
   int v2 = (value / 100) % 10;
   int v3 = (value / 1000) % 10;
   displayValues(v0, v1, v2, v3);
  delay(1);
}
 
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


void displayValues(int v0, int v1, int v2, int v3) {  
  long start = millis();
  long current = start;
  while (current < start + 50 && current >= start) {
    
    current = millis();
    for (int i = 0; i < 10; i++) {
      digitalWrite(latch,LOW);
      // select the fourth segment from left
      shiftOut(data,clock,MSBFIRST,segment[v0]);
      // show the digit "5" ( the array starts with 0 to count!)
      shiftOut(data,clock,MSBFIRST,B00000001);  
      digitalWrite(latch,HIGH);
      digitalWrite(latch,LOW);
      // select the fourth segment from left
      shiftOut(data,clock,MSBFIRST,segment[v1]);
      // show the digit "5" ( the array starts with 0 to count!)
      shiftOut(data,clock,MSBFIRST,B00000010);  
      digitalWrite(latch,HIGH);
      digitalWrite(latch,LOW);
      // select the fourth segment from left
      shiftOut(data,clock,MSBFIRST,segment[v2]);
      // show the digit "5" ( the array starts with 0 to count!)
      shiftOut(data,clock,MSBFIRST,B00000100);  
      digitalWrite(latch,HIGH);
      digitalWrite(latch,LOW);
      // select the fourth segment from left
      shiftOut(data,clock,MSBFIRST,segment[v3]);
      // show the digit "5" ( the array starts with 0 to count!)
      shiftOut(data,clock,MSBFIRST,B00001000);  
      digitalWrite(latch,HIGH);
    }
  }
}
