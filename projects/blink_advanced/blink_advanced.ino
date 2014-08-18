int led = 13;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  for (int i = 0; i < 16; i++) {
    Serial.print("Probing led ");
    Serial.println(i);
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
    delay(100);
  }
}
