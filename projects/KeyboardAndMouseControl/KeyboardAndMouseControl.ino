// For joystick
int xPin = A2;
int yPin = A3;
int buttonPin = 9;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
long lastReport = 0;
long movementResidueX = 0;
long movementResidueY = 0;
int xNull = 513;
int yNull = 526;

// For rotary encoder

volatile boolean TurnDetected = false;
volatile boolean up = false;

const int PinCLK = 7;                 // Used for generating interrupts using CLK signal
const int PinDT = 5;                  // Used for reading DT signal
const int PinSW = 6;                  // Used for the push button switch
long lastPressed = 0;

void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
  Serial.println("Interrupt");
  if (digitalRead(PinCLK))
    up = digitalRead(PinDT);
  else
    up = !digitalRead(PinDT);
  TurnDetected = true;
}

// Main stuff
void setup() { // initialize the buttons' inputs:

  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  
  digitalWrite(PinSW, HIGH);
  attachInterrupt (4, isr, FALLING); // interrupt 0 is always connected to pin 2 on Arduino UNO
  Serial.begin (9600);
  Serial.println("Start");
  // initialize mouse control:
  Mouse.begin();
  Keyboard.begin();
  lastReport = millis();
}

long virtualPosition = 0;

long toSendX = 0;
long toSendY = 0;
long deltaT = 0;
long totalX = 0;
long totalY = 0;
long coeff = 300;

void loop() {
  // Check joystick
  xPosition = analogRead(xPin) - xNull;
  if (abs(xPosition) < 5) {
    xPosition = 0;
  }
  else if (abs(xPosition) < 100) {
    xPosition = xPosition / 10;
  }
  else if (abs(xPosition) < 200) {
    xPosition = xPosition / 5;
  }
  yPosition = analogRead(yPin) - yNull;
  if (abs(yPosition) < 5) {
    yPosition = 0;
  }
  else if (abs(yPosition) < 100) {
    yPosition = yPosition / 10;
  }
  else if (abs(yPosition) < 200) {
    yPosition = yPosition / 5;
  }
  deltaT = millis() - lastReport;
  if (deltaT > 0) {
    lastReport = millis();
    totalX = xPosition * deltaT + movementResidueX;
    toSendX = totalX / coeff;
    movementResidueX = totalX - toSendX * coeff;
    totalY = yPosition * deltaT + movementResidueY;
    toSendY = totalY / coeff;
    movementResidueY = totalY - toSendY * coeff;
    if (abs(toSendX) > 0 || abs(toSendY) > 0) {
      Mouse.move(toSendX,-toSendY);
    }
  }
  
  // Check rotary click
  if (!(digitalRead(PinSW)) && ((millis() - lastPressed) > 50)) {
    Serial.println ("Click");
    Mouse.click();
  }

  if (TurnDetected)  {		    // do this only if rotation was detected
    if (up) {
      virtualPosition++;
      Mouse.move(0, 0, -1);
    } else {
      virtualPosition--;
      Mouse.move(0, 0, 1);
    }
    TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
    Serial.print ("Count = ");
    Serial.println (virtualPosition);
  }

  /*
    switch (inChar) {
        case 'u':
          // move mouse up
          Mouse.move(0, -40);
          break;
        case 'd':
          // move mouse down
          Mouse.move(0, 40);
          break;
        case 'l':
          // move mouse left
          Mouse.move(-40, 0);
          break;
        case 'r':
          // move mouse right
          Mouse.move(40, 0);
          break;
        case 'm':
          // perform mouse left click
          Mouse.click(MOUSE_LEFT);
          break;
      }
    */

  //    Keyboard.write('u');
}

