#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2
#define BUTTON_PIN 12

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   10

TM1637Display display(CLK, DIO);
int k = 0;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, LOW); // connect internal pull-up
  display.setBrightness(0x0f);
}

void loop()
{
    k++;
  // Show decimal numbers with/without leading zeros
  display.showNumberDec(k, true, 4, 0);
  if (k > 99) {
    clearDisplay();
    k = 0;
  }
  delay(100);
}

void clearDisplay() {
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setSegments(data);
}
