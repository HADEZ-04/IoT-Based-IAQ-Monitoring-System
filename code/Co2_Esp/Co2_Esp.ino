#include <HardwareSerial.h>
#include <MHZ19.h>

#define RXD2 18  // MH-Z19E TX → ESP32 RX
#define TXD2 5  // MH-Z19E RX ← ESP32 TX

HardwareSerial mySerial(1);  // Use UART1
MHZ19 co2Sensor;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  co2Sensor.begin(mySerial);
  Serial.println("MH-Z19E ready on NodeMCU ESP32...");
}

void loop() {
  int co2 = co2Sensor.getCO2();
  Serial.print("CO2: ");
  Serial.print(co2);
  Serial.println(" ppm");
  delay(2000);
}
