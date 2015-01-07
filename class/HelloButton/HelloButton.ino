int buttonPin = 4;
int ledPin = 13; // just using for example
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
}

void loop()
{
  if(digitalRead(buttonPin) == LOW){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(5);
}
