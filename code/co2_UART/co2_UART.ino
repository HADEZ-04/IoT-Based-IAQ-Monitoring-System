#include <SoftwareSerial.h>
#include <MHZ19.h>

SoftwareSerial co2Serial(10, 11); // Arduino RX, TX
MHZ19 co2Sensor;

void setup() {
  Serial.begin(9600);           // Serial monitor
  co2Serial.begin(9600);        // MH-Z19E sensor
  co2Sensor.begin(co2Serial);
  Serial.println("MH-Z19E ready...");
}

void loop() {
  int co2 = co2Sensor.getCO2();
  Serial.print("CO2: ");
  Serial.print(co2);
  Serial.println(" ppm");
  delay(2000);
}
