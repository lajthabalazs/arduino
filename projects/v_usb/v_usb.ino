#include "UsbKeyboard.h"

void setup() {
  // USB setup -----------------
  // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  TIMSK0 &= !(1<TOIE0);
  // Clear interrupts while performing time-critical operations
  cli();

  // Force re-enumeration so the host will detect us
  usbDeviceDisconnect();
  delayMs(250);
  usbDeviceConnect();

  // Set interrupts again
  sei();
}

void loop() {
  // update USB device state
  UsbKeyboard.update();
    UsbKeyboard.sendKeyStroke(KEY_A);
    delay(1000);
}

// helper method for V-USB library
void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}
