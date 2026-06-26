//MZ19 TX 17 
// MZ19 RX 16
//DHT 4
// VOC A 19
//VOC B 18
#include <WiFi.h>             
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <MHZ19.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define RXD2 18  // MH-Z19E TX → ESP32 RX
#define TXD2 5  // MH-Z19E RX ← ESP32 TX

#define RXD3 33  // MH-Z19E TX → ESP32 RX
#define TXD3 32  
#define DHTPIN 23               
#define DHTTYPE DHT11   

int pinA=26; //19
int pinB=25; //

uint8_t buf[32];
uint16_t pm1  = 0;
uint16_t pm25 = 0;
uint16_t pm10 = 0;
volatile float temp = 0; // Celsius
volatile float hum=0;
bool aVal = 0;
bool bVal = 0;
volatile int co2 = 0;
volatile int voc=0;

const char* ssid = "ZFSRM";
const char* password = "ZFLABSRM";
const char* mqtt_server = "192.168.137.57";  

MHZ19 co2Sensor;
StaticJsonDocument<200> doc;
String payload;
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mySerial(1);  // Use UART1
HardwareSerial pmSerial(2);  // Use UART1
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESPClient");
  }
}

void setup() {
Serial.begin(9600);
mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
pmSerial.begin(9600, SERIAL_8N1, RXD3, TXD3);
pinMode(pinA, INPUT);
pinMode(pinB, INPUT);  
co2Sensor.begin(mySerial);
dht.begin();
setup_wifi();
client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
 temp = dht.readTemperature(); 
 hum=dht.readHumidity();
 aVal = digitalRead(pinA);
 bVal = digitalRead(pinB);
 co2 = co2Sensor.getCO2();
 voc=readvoc(aVal,bVal);
  if (pmSerial.available() >= 32)
  {
    pmSerial.readBytes(buf, 32);
    if (buf[0] == 0x42 && buf[1] == 0x4D)
    {
       pm1  = (buf[10] << 8) | buf[11];
       pm25 = (buf[12] << 8) | buf[13];
       pm10 = (buf[14] << 8) | buf[15];
    }
  }
Serial.print(temp);Serial.print("  ");
Serial.print(hum);Serial.print("  ");
Serial.print(co2);Serial.print("  ");
Serial.print(pm1);Serial.print("  ");
Serial.print(pm25);Serial.print("  ");
Serial.print(pm10);Serial.println("  ");

doc["temp"] = temp;
doc["hum"] = hum;
doc["co2"] = co2;
doc["voc"] = voc;
doc["pm1"] = pm1;
doc["pm25"] = pm25;
doc["pm10"] = pm10;
serializeJson(doc, payload);
client.publish("room/iaq", payload.c_str());
client.loop();
delay(30000); 
}


int readvoc(bool aVal,bool bVal){
if (aVal == LOW && bVal == LOW) {
      return 0;
  }
  else if (aVal == LOW && bVal == HIGH) {
      return 25;

  }
  else if (aVal == HIGH && bVal == LOW) {
      return 50;

  }
  else if (aVal == HIGH && bVal == HIGH) {
      return 1000;

  }
  else {
    Serial.println("Invalid reading.");
  }
}