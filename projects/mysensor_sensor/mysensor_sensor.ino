#include <Arduino.h>
#include <MySensor.h>
#include <SPI.h>

#define INCLUSION_MODE_TIME 1 // Number of minutes inclusion mode is enabled
#define INCLUSION_MODE_PIN 3 // Digital pin used for inclusion mode button



#define SENSOR_ID 0
#define OPEN 1
#define CLOSE 0

MySensor gw;
MyMessage msg(SENSOR_ID, V_TRIPPED);

void setup()
{
	gw.begin();
	gw.present(SENSOR_ID, S_DOOR);
}

void loop()
{
  gw.send(msg.set(OPEN));
  delay(10000);              // wait for a second
}
