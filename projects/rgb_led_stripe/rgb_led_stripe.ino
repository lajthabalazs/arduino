int r = 0;
int g = 0;
int b = 0;

int RED_PIN = 11;
int GREEN_PIN = 9;
int BLUE_PIN = 10;

String message;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  readSerial();
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
  delay(10);
}



void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    message += c; //makes the string readString
    delay(5);  //slow looping to allow buffer to fill with next character
  }

  if (message.length() > 0) {
    Serial.println(message);  //so you can see the captured string 
    int commaPosition = message.indexOf(',');
    if(commaPosition != -1) {
      r = (message.substring(0,commaPosition)).toInt();
      int secondCommaPosition = commaPosition + 1 + message.substring(commaPosition + 1, message.length()).indexOf(',');
      g = (message.substring(commaPosition + 1, secondCommaPosition)).toInt();
      b = (message.substring(secondCommaPosition + 1, message.length())).toInt();
      Serial.print("Color : R ");
      Serial.print(r);
      Serial.print(" G ");
      Serial.print(g);
      Serial.print(" B ");
      Serial.println(b);
    }
    message = "";
  } 
}
