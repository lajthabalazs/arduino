// For joystick
int xPin = A2;
int yPin = A3;
int buttonPin = 9;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
unsigned long lastMillis = 0;
unsigned long lastMicros = 0;
unsigned long lastReport = 0;
long movementResidueX = 0;
long movementResidueY = 0;
int xNull = 513;
int yNull = 526;

// Main stuff
void setup() { // initialize the buttons' inputs:

  Serial.begin (9600);
  Serial.println("Start");
  // initialize mouse control:
  Mouse.begin();
  Keyboard.begin();
  lastReport = micros();
  lastMillis = lastReport / 1000;
  lastMicros = lastReport - lastReportMillis * 1000;
}

unsigned long newMicros;
unsigned long nowMillis;
unsigned long deltaMicros;

long getEllapsedMicros() { // Advances time to current time
  nowMicros = micros();
  if (nowMicros < lastReport) { // Micros overflow
    deltaMicros = nowMicros + 
  } else {
    deltaMicros = nowMicros - lastReport;
  } 
}

long toSendX = 0;
long toSendY = 0;
long deltaT = 0;
long totalX = 0;
long totalY = 0;
long coeff = 300;

void loop() {
  // Check joystick
  xPosition = analogRead(xPin) - xNull;
  yPosition = analogRead(yPin) - yNull;
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
}

