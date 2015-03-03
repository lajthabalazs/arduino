void setup()
{
  Serial.begin(9600);
  Serial.println("Szamologep v1.0");
  Serial.setTimeout(100);
}

void loop()
{
  Serial.println("Add meg milyen hosszu tombot akarsz hasznalni, majd <Enter>!");
  Serial.print("A tomb hossza : ");
  int dataLength = parseInteger();
  Serial.println(dataLength);
  Serial.println("Add meg a tomb elemeit, szokozzel elvalasztva, majd <Enter>!");
  Serial.print("Szamok : [" );
  int data[dataLength];
  for (int i = 0; i < dataLength; i++) {
    if (i > 0) {
      Serial.print(", ");
    }
    data[i] = parseInteger();
    Serial.print(data[i]);
  }
  Serial.println("]");
  // TODO you have your numbers, do what you want!
  // Clear serial
  while (Serial.available() > 0) {  // .parseInt() can leave non-numeric characters
    char junk = Serial.read();     // clear the keyboard buffer
    delay(5);
  }
  delay(1000);
}

String inString = "";

int parseInteger() {
  while (Serial.available() == 0) ;
  int i = Serial.parseInt();
  while (Serial.available() > 0) {  // .parseInt() can leave non-numeric characters
    char junk = Serial.read();     // clear the keyboard buffer
    delay(1);
  }
  return i;
}
