#include <DHT.h>

#include <PubSubClient.h> // For MQTT

#include <SPI.h> // For Ethernet
#include <Dhcp.h> // For Ethernet
#include <Dns.h> // For Ethernet
#include <Ethernet.h> // For Ethernet
#include <EthernetClient.h> // For Ethernet
#include <EthernetServer.h> // For Ethernet
#include <EthernetUdp.h> // For Ethernet

#include <LiquidCrystal_I2C.h> // For I2C LCD
#include <Wire.h> // For I2C LCD

#define MQTT_SERVER "smarthome.tmit.bme.hu"

#define DHTPIN 2
#define DHTTYPE DHT11

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };

EthernetClient ethernetClient;
PubSubClient sensorClient(MQTT_SERVER, 1883, sensorCallback, ethernetClient);
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin (115200);
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  int i = 0;
  while (true) {
    lcd.print("Trying...");
    lcd.setCursor(9,0);
    lcd.print(i);
    delay(500);
    // initialize ethernet library  
    if (Ethernet.begin(MAC_ADDRESS) != 0) {
      clearLine(0);
      lcd.print(Ethernet.localIP());
      Serial.print("Ethernet using DHCP: ");
      Serial.println(Ethernet.localIP());
      if (sensorClient.connect("ethernetnode01")) {
        clearLine(1);
        lcd.print("MQTT connected");
        Serial.print("MQTT connected");
        delay(1000);
        if(sensorClient.subscribe("ethernet/01/command")) {
          clearLine(1);
          lcd.print("Subscribed");
          Serial.print("Subscribed");
          delay(1000);
          break;
        } else {
          clearLine(1);
          lcd.print("Subsc failed");
          Serial.print("Subsc failed");
        }
      } else {
        clearLine(1);
        lcd.print("MQTT failed");
        Serial.print("MQTT failed");
      }
    } else {
      clearLine(1);
      lcd.print("No DHCP!");
      Serial.println("Failed to configure Ethernet using DHCP");
    }
  }
}  // end of setup

void clearLine(int line) {
    lcd.setCursor(0,line);
    lcd.print("                ");
    lcd.setCursor(0,line);
}

long lastTime = millis();
int messageDelay = 5000;
float humidity = 0;
float temperature = 0;

void loop() {
  sensorClient.loop();
  if (millis() > lastTime + messageDelay) {
    clearLine(1);
    lcd.print("Reading sensors");
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();  
    sendMessage();
    lastTime = millis();
  }
  delay(1);
}

long getDecimal(float val) {
 int intPart = int(val);
 long decPart = 10 * (val - intPart); 
 if(decPart > 0) {
   return(decPart);
 } else if(decPart < 0) {
   return((-1)*decPart);
 } else if(decPart = 0){ 
   return(0);
 }
}

void sendMessage() {
  Serial.print("Publishing...");
  
  String tempString = String(int(temperature))+ "."+String(getDecimal(temperature));
  String humidityString = String(int(humidity))+ "."+String(getDecimal(humidity));
  char buff[7];  
  for (int i = 0; i < 7; i++) {
    buff[i] = ' ';
  }
  tempString.toCharArray(buff,7);
  sensorClient.publish("ethernet/01/temp", buff);
  for (int i = 0; i < 7; i++) {
    buff[i] = ' ';
  }
  humidityString.toCharArray(buff,7);
  sensorClient.publish("ethernet/01/humidity", buff);
}

void sensorCallback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Data received");
  char message[length+1];
  getString(payload, message, length);
  clearLine(1);  
  lcd.print(message);
  Serial.println(message);
}

char* getString(byte bytes[], char chars[], byte len) {
  for (byte i = 0; i < len; i++) {
    chars[i] = bytes[i];
  }  
  chars[len] = '\0';
  return chars;
}
 
// handles message arrived on subscribed topic(s)
// in this application we do not subscribe so this is a dummy method
void provisionCallback(char* topic, byte* payload, unsigned int length) { }
