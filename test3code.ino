// ======================================================
//              ESP32 SMART ENERGY METER
// ======================================================
//
// ZMPT101B Voltage Sensor  -> GPIO 34
// Current Sensor            -> GPIO 35
// Relay S/IN               -> GPIO 32
//
// Serial Monitor: 115200 baud
// ======================================================

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define RELAY_PIN   32

// ------------------------------------------------------
// CALIBRATION
// ------------------------------------------------------
// These MUST be calibrated for your actual sensors.
//
// Start with these values and we will adjust them later.

float voltageCalibration = 1.0;
float currentCalibration = 1.0;

// Current sensor zero offset.
// For an ACS712-type sensor, this is approximately
// the ADC reading corresponding to zero current.
// The program automatically measures it at startup.

float currentOffset = 0;

// ------------------------------------------------------
// ENERGY
// ------------------------------------------------------

double energy_kWh = 0.0;

unsigned long previousTime;


// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(RELAY_PIN, OUTPUT);

  // Relay initially OFF
  digitalWrite(RELAY_PIN, LOW);

  Serial.println();
  Serial.println("======================================");
  Serial.println("       ESP32 SMART ENERGY METER");
  Serial.println("======================================");

  delay(1000);

  // ----------------------------------------------------
  // Find current sensor zero offset
  // ----------------------------------------------------

  Serial.println("Calibrating current sensor...");
  Serial.println("Make sure NO current is flowing.");

  long sum = 0;

  for (int i = 0; i < 2000; i++) {
    sum += analogRead(CURRENT_PIN);
    delayMicroseconds(200);
  }

  currentOffset = sum / 2000.0;

  Serial.print("Current zero offset = ");
  Serial.println(currentOffset);

  previousTime = millis();

  Serial.println("Measurement started.");
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  const int samples = 2000;

  double voltageSquareSum = 0;
  double currentSquareSum = 0;

  // ----------------------------------------------------
  // Sample voltage and current waveforms
  // ----------------------------------------------------

  for (int i = 0; i < samples; i++) {

    int voltageRaw = analogRead(VOLTAGE_PIN);
    int currentRaw = analogRead(CURRENT_PIN);

    // Remove DC midpoint
    double voltageAC = voltageRaw - 2048.0;
    double currentAC = currentRaw - currentOffset;

    voltageSquareSum += voltageAC * voltageAC;
    currentSquareSum += currentAC * currentAC;

    delayMicroseconds(200);
  }

  // ----------------------------------------------------
  // RMS ADC values
  // ----------------------------------------------------

  double voltageRMS_ADC =
      sqrt(voltageSquareSum / samples);

  double currentRMS_ADC =
      sqrt(currentSquareSum / samples);

  // ----------------------------------------------------
  // Convert to actual values
  // ----------------------------------------------------

  double voltage =
      voltageRMS_ADC * voltageCalibration;

  double current =
      currentRMS_ADC * currentCalibration;

  // ----------------------------------------------------
  // Apparent power
  // ----------------------------------------------------

  double apparentPower = voltage * current;

  // ----------------------------------------------------
  // Energy calculation
  // ----------------------------------------------------

  unsigned long currentTime = millis();

  double elapsedHours =
      (currentTime - previousTime) / 3600000.0;

  energy_kWh +=
      (apparentPower * elapsedHours) / 1000.0;

  previousTime = currentTime;

  // ----------------------------------------------------
  // DISPLAY
  // ----------------------------------------------------

  Serial.println();
  Serial.println("--------------------------------------");

  Serial.print("Voltage RMS : ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  Serial.print("Current RMS : ");
  Serial.print(current, 3);
  Serial.println(" A");

  Serial.print("Power       : ");
  Serial.print(apparentPower, 2);
  Serial.println(" VA");

  Serial.print("Energy      : ");
  Serial.print(energy_kWh, 6);
  Serial.println(" kWh");

  Serial.print("Relay       : ");

  if (digitalRead(RELAY_PIN) == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  Serial.println("--------------------------------------");

  delay(1000);
}
