// For joystick
int xPin = A2;
int yPin = A3;

int xPosition = 0;
int yPosition = 0;
int xNull = 509;
int yNull = 496;

// Main stuff
void setup() { // initialize the buttons' inputs:

  Serial.begin (9600);
  Serial.println("Start");
  // initialize mouse control:
  Mouse.begin();
}

long toSendX = 0;
long toSendY = 0;
long coeff = 100;

void loop() {
  // Check joystick
  xPosition = analogRead(xPin) - xNull;
  yPosition = analogRead(yPin) - yNull;

 // lastReport = millis();
  Serial.print(xPosition);
  Serial.print(";");
  Serial.println(yPosition);    
  toSendX = xPosition / coeff;
  toSendY = yPosition / coeff;
  if (abs(toSendX) > 0 || abs(toSendY) > 0) {
    Mouse.move(toSendX,-toSendY);
  }
  delay(1);
}

