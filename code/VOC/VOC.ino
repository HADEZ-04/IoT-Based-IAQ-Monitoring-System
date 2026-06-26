// Define sensor output pins
const int pinA = 5;  // Connect A output of sensor to digital pin 2
const int pinB = 3;  // Connect B output of sensor to digital pin 3

void setup() {
  Serial.begin(9600);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  Serial.println("Warming up sensor (wait 3-5 mins)...");
  // delay(30000); // 5-minute warm-up (in milliseconds)
  Serial.println("Sensor ready. Reading air quality...");
}

void loop() {
  int aVal = digitalRead(pinA);
  int bVal = digitalRead(pinB);

  if (aVal == LOW && bVal == LOW) {
    Serial.println("Air Quality: No Pollution");
  }
  else if (aVal == LOW && bVal == HIGH) {
    Serial.println("Air Quality: Slight Pollution");
  }
  else if (aVal == HIGH && bVal == LOW) {
    Serial.println("Air Quality: Middle Pollution");
  }
  else if (aVal == HIGH && bVal == HIGH) {
    Serial.println("Air Quality: Heavy Pollution");
  }
  else {
    Serial.println("Invalid reading.");
  }

  delay(1000); // Read every 1 second
}