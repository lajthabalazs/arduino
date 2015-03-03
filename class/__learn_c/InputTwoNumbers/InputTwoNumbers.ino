void setup()
{
  Serial.begin(9600);
  Serial.println("Szamologep v1.0");
  Serial.setTimeout(100);
}

void loop()
{
  Serial.println("Irj be ket szamot koztuk szokoz, majd <Enter>!");
  Serial.print("i : ");
  int i = parseInteger();
  Serial.println(i);
  Serial.print("j : " );
  int j = parseInteger();
  Serial.println(j);
  // TODO you have your numbers, do what you want!
  delay(1000);
}

String inString = "";

int parseInteger() {
  while (Serial.available() == 0) ;
  int i = Serial.parseInt();
  while (Serial.available() > 0)  // .parseInt() can leave non-numeric characters
  { char junk = Serial.read() ; }      // clear the keyboard buffer
  return i;
}
