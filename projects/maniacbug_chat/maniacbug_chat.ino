#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

# string.getBytes(buf, len)
# Serial.readString(); 

RF24 radio(9,10);

uint64_t addressBase = 0xF0F0F0F000LL;

uint64_t local;

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  printf("Welcome to NRF chat!\n\r");
  printf("*** Write local address's last byte\n\r");
  while(!Serial.available()) {
    delay(10);
  }
  local = addressBase + Serial.parseInt();
  printf("Local address is %lu...\n\r",local);
  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,local);
  radio.startListening();
  radio.printDetails();
}
uint64_t destination;
boolean hasMessage = false;
boolean firstTimeout = true;

void loop(void) {
  if (hasMessage) {
    hasMessage = false;
    radio.stopListening();
    radio.openWritingPipe(addressBase + destination);
    printf("Sending message to %lu...\n\r",destination);
    int data = destination;
    bool ok = radio.write( &data, sizeof(unsigned int) );
    if (ok) {
      printf("ok...");
    } else {
      printf("failed.\n\r");
    }
    radio.openReadingPipe(1,local);
    Serial.println("Listening...");
    radio.startListening();
  }
  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 200 )
      timeout = true;
  // Describe the results
  if ( timeout ) {
    if (firstTimeout) {
      firstTimeout = false;
      Serial.print("No message received");
    } else {
      Serial.print(".");
    }
  }
  else {
    firstTimeout = true;
    Serial.println("");
    unsigned long got_time;
    radio.read( &got_time, sizeof(unsigned long) );
    Serial.println();
    printf("Got message %lu\n\r", got_time);
  }
  if (Serial.available()) {
    destination = Serial.parseInt();
    hasMessage = true;
  }
}
