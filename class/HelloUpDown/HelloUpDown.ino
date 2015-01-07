int ledPin = 13;
int upButtonPin = 4;
int downButtonPin = 7;
boolean upPressed = false;
boolean downPressed = false;
boolean ledOn = false;

void setup()
{
  pinMode(upButtonPin, INPUT);
  digitalWrite(upButtonPin, HIGH);
  pinMode(downButtonPin, INPUT);
  digitalWrite(downButtonPin, HIGH);
}

void loop()
{
  if(digitalRead(upButtonPin) == LOW) {
    if (upPressed == false) {
      ledOn = true;
    }
    upPressed = true;
  } else {
    upPressed = false;
  }
  if(digitalRead(downButtonPin) == LOW) {
    if (downPressed == false) {
      ledOn = false;
    }
    downPressed = true;
  } else {
    downPressed = false;
  }
  delay(5);
  if (ledOn) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
