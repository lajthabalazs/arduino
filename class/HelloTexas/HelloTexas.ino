#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures();
  int temp = (int)sensors.getTempCByIndex(0);
}
