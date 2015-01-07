int led = 13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  dot();
  dot();
  dot();
  delay(200);
  dash();
  dash();
  dash();
  delay(200);
  dot();
  dot();
  dot();
  delay(600);
}

void dot() {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

void dash() {
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(100);
}
