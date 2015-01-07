int led = 13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    dot();
  }
  delay(200);
  for (int i = 0; i < 3; i++) {
    dash();
  }
  delay(200);
  for (int i = 0; i < 3; i++) {
    dot();
  }
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
