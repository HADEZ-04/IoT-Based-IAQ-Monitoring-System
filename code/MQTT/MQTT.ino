//MZ19 TX 17 
// MZ19 RX 16
//DHT 4
// VOC A 19
//VOC B 18

#include <WiFi.h>             
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <MHZ19.h>
#include "DHT.h"

#define RXD2 17  // MH-Z19E TX → ESP32 RX
#define TXD2 16  // MH-Z19E RX ← ESP32 TX

volatile float temp = 0; // Celsius
volatile float hum=0;
volatile int aVal = 0;
volatile int bVal = 0;
volatile int co2 = 0;
volatile float voc=0;
HardwareSerial mySerial(1);  // Use UART1
MHZ19 co2Sensor;

int pinA=25; //19
int pinB=26; //
#define DHTPIN 4               
#define DHTTYPE DHT11          
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "VijayV";
const char* password = "12345678";
const char* mqtt_server = "192.168.149.27";  

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
Serial.begin(115200);
mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
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

client.loop();
 temp = dht.readTemperature(); 
 hum=dht.readHumidity();
 aVal = digitalRead(pinA);
 bVal = digitalRead(pinB);
 co2 = co2Sensor.getCO2();
 voc=0;

  if (!isnan(temp)) {
    client.publish("room/temperature", String(temp).c_str());  // Publish to topic
    client.publish("room/humidity", String(hum).c_str());  // Publish to topic
    Serial.println("Temperature: " +  String(temp) + " °C");     // Debug print
  } else {
    Serial.println("Failed to read from DHT11");
  }

    if (aVal == LOW && bVal == LOW) {
      voc=0;
    client.publish("room/Voc", String(voc).c_str());  // Publish to topic
    Serial.println("Air Quality: No Pollution");
  }
  else if (aVal == LOW && bVal == HIGH) {
    voc=25;
    client.publish("room/Voc", String(voc).c_str());  // Publish to topic
    Serial.println("Air Quality: Slight Pollution");
  }
  else if (aVal == HIGH && bVal == LOW) {
    voc=50;
    client.publish("room/Voc", String(voc).c_str());  // Publish to topic

    Serial.println("Air Quality: Middle Pollution");
  }
  else if (aVal == HIGH && bVal == HIGH) {
    voc=100;
    client.publish("room/Voc", String(voc).c_str());  // Publish to topic

    Serial.println("Air Quality: Heavy Pollution");
  }
  else {
    Serial.println("Invalid reading.");
  }

  if (!isnan(co2)) {
    client.publish("room/Co2", String(co2).c_str());  // Publish to topic
    Serial.println("Co2: " +  String(co2) + " °C");     // Debug print
  } else {
    Serial.println("Failed to read from MHZ19");
  }
  delay(2000); // Update every 2 seconds (adjust for your real-time needs)
}