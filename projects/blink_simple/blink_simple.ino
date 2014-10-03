// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("3 to 13 up");
  for (int i = 3; i <= 13; i++) {
    digitalWrite(i, HIGH);
  }
  delay(1000);
  Serial.println("3 to 13 down");
  for (int i = 3; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
  delay(100);
}
