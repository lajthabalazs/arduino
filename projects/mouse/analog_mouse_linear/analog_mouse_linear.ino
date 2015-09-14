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

// Main stuff
void setup() { // initialize the buttons' inputs:

  Serial.begin (9600);
  Serial.println("Start");
  xPosition = 0;
  yPosition = 0;
  int cycles = 10;
  for (int i = 0; i < cycles; i++) {
    xPosition += analogRead(xPin);
    yPosition += analogRead(yPin);
    delay(10);
  }
  xNull = xPosition / cycles;
  yNull = yPosition / cycles;
  Serial.print("Initialized ");
  Serial.print(xNull);
  Serial.print(" , ");  
  Serial.print(yNull);
  // initialize mouse control:
  Mouse.begin();
  Keyboard.begin();
  lastReport = millis();
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
