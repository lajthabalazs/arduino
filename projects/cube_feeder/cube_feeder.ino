#include <ICSC.h>

char clearData[] = "Hello";
char RGBData[] = "RRRRRRRRRGGGGGGGGGBBBBBBBBB";

void setup() {
  pinMode(13, OUTPUT);
  ICSC.begin(4, 115200);
}

void loop() {
  digitalWrite(13, HIGH);
  ICSC.send(3, 'D', 27, (char *)&RGBData);
  delay(1000);
}

void sendTestSequence() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    ICSC.send(3, 'T', 3, (char *)&RGBData);
    delay(1000);
    digitalWrite(13, LOW);
    ICSC.send(3, 'C', 5, (char *)&clearData);
    delay(1000);
  }
}
