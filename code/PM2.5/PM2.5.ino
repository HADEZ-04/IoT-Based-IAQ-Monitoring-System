#include <HardwareSerial.h>
#include <MHZ19.h>

#define RXD2 33  // MH-Z19E TX → ESP32 RX
#define TXD2 32  // MH-Z19E RX ← ESP32 TX
uint8_t buf[32];

HardwareSerial pmSerial(2);  // Use UART1

void setup() {
  Serial.begin(115200);
  pmSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

}
void loop()
{
  if (pmSerial.available() >= 32)
  {
    pmSerial.readBytes(buf, 32);

    if (buf[0] == 0x42 && buf[1] == 0x4D)
    {
      uint16_t pm1  = (buf[10] << 8) | buf[11];
      uint16_t pm25 = (buf[12] << 8) | buf[13];
      uint16_t pm10 = (buf[14] << 8) | buf[15];

      Serial.print("PM1.0: "); Serial.println(pm1);
      Serial.print("PM2.5: "); Serial.println(pm25);
      Serial.print("PM10 : "); Serial.println(pm10);
      Serial.println();
    }
  }
}
