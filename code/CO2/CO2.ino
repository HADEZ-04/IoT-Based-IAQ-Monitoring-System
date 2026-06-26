#include <MHZ19.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10); // RX, TX
MHZ19 myMHZ19;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  myMHZ19.begin(mySerial);
  // myMHZ19.autoCalibration();
  // delay(30000);
}

void loop() {
  int co2 = myMHZ19.getCO2();

  Serial.print("CO2 Concentration: ");
  Serial.print(co2);
  Serial.println(" ppm");

  delay(1000);
}
