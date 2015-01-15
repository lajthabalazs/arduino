void setup()
{
  Serial.begin(9600);
  Serial.println("Szamologep v1.0");
  Serial.setTimeout(100);
}

void loop()
{
  Serial.println("Kerek egy szamot, majd <Enter>!");
  Serial.print("i : ");
  int i = parseInteger();
  Serial.println(i);
  Serial.println("Kerek meg egy szamot, majd <Enter>!");
  Serial.print("j : " );
  int j = parseInteger();
  Serial.println(j);
  int result = i + j;
  Serial.print("Az eredmeny i = ");
  Serial.print(i);
  Serial.print(", j = ");
  Serial.print(j);
  Serial.print(", i + j = ");
  Serial.println(result);
  delay(2000);
}

String inString = "";

int parseInteger() {
  while (Serial.available() == 0) ;
  int i = Serial.parseInt();
  while (Serial.available() > 0)  // .parseInt() can leave non-numeric characters
  { char junk = Serial.read() ; }      // clear the keyboard buffer
  return i;
}
