#include <LedControl.h>

LedControl lc1=LedControl(12,11,10,9); 

void setup() {
  // put your setup code here, to run once:
  for(int index=0;index<lc1.getDeviceCount();index++) {
      lc1.shutdown(index,false);
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  lc1.setLed(0,2,7,true);
  lc1.setLed(0,3,6,true);
  lc1.setLed(0,4,5,true);
  lc1.setLed(0,5,4,true);  
  delay(1000);
}
