#include <Wire.h>

int nextAddress = 51;
int dataReceived = 0;
boolean sendRequest = false;
String message;

void setup()
{
  Serial.begin(9600);
  Wire.begin(30); // Start I2C Bus as Master
  Wire.onReceive(receiveEvent); // register event
}
void loop()
{
  if (sendRequest) {
    sendRequest = false;
    Wire.beginTransmission(50);
    Wire.write(nextAddress);
    Wire.endTransmission();
    Serial.println("Sent address");
    Serial.println(nextAddress);
    nextAddress ++;
  }
  
  readSerial();
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
    int commaPosition = message.indexOf(' ');
    if(commaPosition != -1) {
      int n = (message.substring(0,commaPosition)).toInt();
      int data = (message.substring(commaPosition + 1, message.length())).toInt();
      Serial.print("Sending message to : ");
      Serial.println(n);
      delay(5);
      Wire.beginTransmission(n);
      Wire.write(data);
      Wire.endTransmission();
      delay(5);
    }
    message = "";
  } 
}

void receiveEvent(int howMany)
{
  Serial.println("Message received");
  dataReceived = Wire.read();    // receive byte as an integer
  Serial.println(dataReceived);
  sendRequest = true;
}
