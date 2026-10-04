#define VOLTAGE_PIN 34

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int voltage = analogRead(VOLTAGE_PIN);

  Serial.print("Voltage ADC = ");
  Serial.println(voltage);

  delay(500);
}
