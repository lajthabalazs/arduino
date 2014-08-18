#include <Wire.h>

int nextAddress = 51;
int dataReceived = 0;
boolean sendRequest = false;

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
  delay(10);
}

void receiveEvent(int howMany)
{
  Serial.println("Message received");
  dataReceived = Wire.read();    // receive byte as an integer
  Serial.println(dataReceived);
  sendRequest = true;
}
