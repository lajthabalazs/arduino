void setup()
{
  Serial.begin(9600);
  Serial.println("Szamologep v1.0");
  Serial.setTimeout(100);
}

void loop()
{
  Serial.println("Addj meg karaktereket, majd <Enter>!");
  // TODO you have your numbers, do what you want!
  while(Serial.available() == 0){
    delay(1);
  }
  while(Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    // TODO do something with c
    delay(5);
  }
  Serial.println();
  delay(1000);
}
