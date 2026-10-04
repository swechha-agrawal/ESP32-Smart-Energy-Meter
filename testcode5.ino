#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int voltage = analogRead(VOLTAGE_PIN);
  int current = analogRead(CURRENT_PIN);

  Serial.print("Voltage ADC = ");
  Serial.print(voltage);

  Serial.print("    Current ADC = ");
  Serial.println(current);

  delay(500);
}
