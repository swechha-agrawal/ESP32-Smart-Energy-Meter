#define VOLTAGE_PIN 34

// Change this after calibration
float calibration = 1.0;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(VOLTAGE_PIN, ADC_11db);

  delay(1000);
  Serial.println("AC Voltage Measurement");
}

void loop() {

  const int samples = 1000;

  double sum = 0;
  double sumSquares = 0;

  // Take samples
  for (int i = 0; i < samples; i++) {

    int adc = analogRead(VOLTAGE_PIN);

    sum += adc;
    sumSquares += (double)adc * adc;

    delayMicroseconds(200);
  }

  // Calculate mean
  double mean = sum / samples;

  // Calculate AC RMS component
  double variance = (sumSquares / samples) - (mean * mean);

  if (variance < 0)
    variance = 0;

  double rmsADC = sqrt(variance);

  // ESP32 ADC approximately 0–3.3 V
  double rmsVoltageAtADC = (rmsADC / 4095.0) * 3.3;

  // ZMPT calibration
  double mainsVoltage = rmsVoltageAtADC * calibration;

  Serial.print("ADC RMS: ");
  Serial.print(rmsADC, 2);

  Serial.print(" | ADC Voltage RMS: ");
  Serial.print(rmsVoltageAtADC, 3);

  Serial.print(" V | AC Voltage: ");
  Serial.print(mainsVoltage, 2);

  Serial.println(" V");

  delay(1000);
}