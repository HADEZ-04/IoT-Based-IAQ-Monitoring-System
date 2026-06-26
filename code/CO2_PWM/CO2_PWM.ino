#define PWM_PIN 5  // Replace with your actual GPIO

unsigned long tl, th;
float co2;

void setup() {
  Serial.begin(115200);
  pinMode(PWM_PIN, INPUT);
}

void loop() {
  tl = pulseIn(PWM_PIN, LOW, 2000000);  // wait for LOW pulse
  th = pulseIn(PWM_PIN, HIGH, 2000000); // then wait for HIGH pulse

  if (tl == 0 || th == 0) {
    Serial.println("Timeout or sensor error");
    delay(1000);
    return;
  }

  float tl_ms = tl / 1000.0;
  float th_ms = th / 1000.0;

  // Apply MH-Z19E PWM formula (usually offset is 2ms)
  co2 = 2000.0 * (tl_ms - 2.0) / (th_ms + tl_ms);

  Serial.print("CO2 (PWM): ");
  Serial.print(co2);
  Serial.println(" ppm");

  delay(1000);  // Let the next PWM cycle come in
}

