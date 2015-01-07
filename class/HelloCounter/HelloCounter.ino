int buttonPin = 4;
boolean pressed = false;
int pressedCount = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
}

void loop()
{
  if(digitalRead(buttonPin) == LOW) {
    if (pressed == false) {
      pressedCount = pressedCount + 1;
      Serial.print("Button pressed:");
      Serial.println(pressedCount);
    }
    pressed = true;
  } else {
    pressed = false;
  }
  delay(5);
}
