int buttonPin = 4;
int ledPin = 13; // just using for example
boolean pressed = false;
boolean ledOn = false;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
}

void loop()
{
  if(digitalRead(buttonPin) == LOW) {
    if (pressed == false) {
      ledOn = !ledOn;
    }
    pressed = true;
  } else {
    pressed = false;
  }
  delay(5);
  if (ledOn) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
