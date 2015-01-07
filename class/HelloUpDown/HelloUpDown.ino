int upButtonPin = 4;
int downButtonPin = 7;
boolean upPressed = false;
boolean downPressed = false;
int value = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(upButtonPin, INPUT);
  digitalWrite(upButtonPin, HIGH);
  pinMode(downButtonPin, INPUT);
  digitalWrite(downButtonPin, HIGH);
}

void loop()
{
  if(digitalRead(upButtonPin) == LOW) {
    if (upPressed == false) {
      value = value + 1;
      Serial.print("Value incremented: ");
      Serial.println(value);
    }
    upPressed = true;
  } else {
    upPressed = false;
  }
  if(digitalRead(downButtonPin) == LOW) {
    if (downPressed == false) {
      value = value - 1;
      Serial.print("Value decremented: ");
      Serial.println(value);
    }
    downPressed = true;
  } else {
    downPressed = false;
  }
  delay(5);
}
