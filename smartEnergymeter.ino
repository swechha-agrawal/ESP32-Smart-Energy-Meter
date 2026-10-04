#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define RELAY_PIN   32

float voltage;
float current;
float power;

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);   // Relay ON

  Serial.println("Smart Energy Meter Started");
}

void loop() {

  int rawVoltage = analogRead(VOLTAGE_PIN);
  int rawCurrent = analogRead(CURRENT_PIN);

  // Convert ADC values to actual values (approximate)
  voltage = (rawVoltage / 4095.0) * 250.0;   // 0–250V
  current = (rawCurrent - 2048) * 0.01;      // Adjust factor later

  if (current < 0) current = 0;

  power = voltage * current;

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  Serial.println("--------------------");

  // Overload protection
  if (current > 2.0) {
    digitalWrite(RELAY_PIN, HIGH); 
    delay(2000);  // Relay OFF
    Serial.println("Overload! Relay OFF");
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Relay ON
    delay(2000);
  }
}









