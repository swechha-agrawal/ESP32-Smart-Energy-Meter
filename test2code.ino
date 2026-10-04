// ESP32 SMART ENERGY METER
// ZMPT101B  -> GPIO 34
// Current sensor -> GPIO 35
// Relay -> GPIO 32

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define RELAY_PIN   32

// ----------------------------
// Calibration values
// ----------------------------
// These are STARTING values only.
// They must be calibrated with a known multimeter/load.

float voltageCalibration = 1.0;
float currentCalibration = 1.0;

// Energy
float energy_kWh = 0.0;

unsigned long previousTime = 0;

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(RELAY_PIN, OUTPUT);

  // Change HIGH/LOW if your relay works opposite
  digitalWrite(RELAY_PIN, LOW);

  previousTime = millis();

  Serial.println("================================");
  Serial.println("      SMART ENERGY METER");
  Serial.println("================================");
}

void loop() {

  // ----------------------------
  // Read sensors
  // ----------------------------

  long voltageSum = 0;
  long currentSum = 0;

  const int samples = 1000;

  for (int i = 0; i < samples; i++) {

    voltageSum += analogRead(VOLTAGE_PIN);
    currentSum += analogRead(CURRENT_PIN);

    delayMicroseconds(200);
  }

  float voltageRaw = voltageSum / (float)samples;
  float currentRaw = currentSum / (float)samples;


  // ----------------------------
  // Convert sensor readings
  // ----------------------------

  float voltage = voltageRaw * voltageCalibration;
  float current = currentRaw * currentCalibration;


  // ----------------------------
  // Calculate power
  // ----------------------------

  float power = voltage * current;


  // ----------------------------
  // Calculate energy
  // ----------------------------

  unsigned long currentTime = millis();

  float elapsedHours =
      (currentTime - previousTime) / 3600000.0;

  energy_kWh += (power * elapsedHours) / 1000.0;

  previousTime = currentTime;


  // ----------------------------
  // Display results
  // ----------------------------

  Serial.println("--------------------------------");

  Serial.print("Voltage : ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current : ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power   : ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("Energy  : ");
  Serial.print(energy_kWh, 6);
  Serial.println(" kWh");

  Serial.println("--------------------------------");

  delay(1000);
}
