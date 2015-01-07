int i = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
}

void loop() {
  i = i + 1;
  Serial.println(i);
  delay(1000);
}
