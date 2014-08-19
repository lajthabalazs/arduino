#include <TM1637Display.h>
#include <Wire.h>

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2
#define BUTTON_PIN 12

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   10

TM1637Display display(CLK, DIO);
int address = 0;
boolean waitingForAddress = false;
int data = 0;
boolean buttonPressed = false;
long lastButtonEventTime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, LOW); // connect internal pull-up
  display.setBrightness(0x0f);
  Wire.begin(50); // Start I2C Bus as Master
  Wire.onReceive(receiveAddress); // register event
}

void loop()
{
  if (checkButton() && address == 0) {
    // Send request to master
    Serial.println("Address requested");
    waitingForAddress = true;
    delay(10);
    Wire.beginTransmission(30);
    Wire.write(77);
    Wire.endTransmission();
    Serial.println("Sent address request");
  }
  // Show decimal numbers with/without leading zeros
  display.showNumberDec(address * 100 + data, true, 4, 0);
  delay(10);
}

int checkButton()
{
  boolean buttonPressedNow = (digitalRead(BUTTON_PIN) == 1);
  if (buttonPressedNow == 1 && !buttonPressed && millis() > lastButtonEventTime + 10) {
    lastButtonEventTime = millis();
    buttonPressed = true;
    return 1;
  }
  if (buttonPressedNow == 0 && buttonPressed && millis() > lastButtonEventTime + 10) {
    lastButtonEventTime = millis();
    buttonPressed = false;
    return 0;
  }
  return 0;
}

void clearDisplay() {
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setSegments(data);
}

void receiveAddress(int howMany)
{
  int receivedAddress = Wire.read();    // receive byte as an integer
  Serial.println("Address received");
  if (waitingForAddress) {
    waitingForAddress = false;
    address = receivedAddress;
    Serial.println("My address");
    Serial.println(address);
    Wire.begin(address); // Start I2C Bus as Master
    Wire.onReceive(receiveMessage); // register event  
  } else {
    Serial.println("Not my address");
  }
}

void receiveMessage(int howMany)
{
  Serial.println("Message received");
  data = Wire.read();    // receive byte as an integer
  Serial.println(data);
}
