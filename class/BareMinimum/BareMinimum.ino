int i = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
}

void loop() {
  Serial.println(i);
  delay(1000);
}
