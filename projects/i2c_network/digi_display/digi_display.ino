#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2
#define BUTTON_PIN 12

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   10

TM1637Display display(CLK, DIO);
int k = 0;
boolean buttonPressed = false;
long lastButtonEventTime = 0;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, LOW); // connect internal pull-up
  display.setBrightness(0x0f);
}

void loop()
{
  if (checkButton()) {
    k++;
  }
  // Show decimal numbers with/without leading zeros
  display.showNumberDec(k, true, 4, 0);
  if (k > 9999) {
    clearDisplay();
    k = 0;
  }
  delay(1);
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
