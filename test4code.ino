#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {

  int vMin = 4095;
  int vMax = 0;
  long vSum = 0;

  int cMin = 4095;
  int cMax = 0;
  long cSum = 0;

  const int samples = 1000;

  for (int i = 0; i < samples; i++) {

    int v = analogRead(VOLTAGE_PIN);
    int c = analogRead(CURRENT_PIN);

    vSum += v;
    cSum += c;

    if (v < vMin) vMin = v;
    if (v > vMax) vMax = v;

    if (c < cMin) cMin = c;
    if (c > cMax) cMax = c;

    delayMicroseconds(200);
  }

  Serial.println("------------------------");

  Serial.print("Voltage Avg : ");
  Serial.println(vSum / (float)samples);

  Serial.print("Voltage Min : ");
  Serial.println(vMin);

  Serial.print("Voltage Max : ");
  Serial.println(vMax);

  Serial.print("Current Avg : ");
  Serial.println(cSum / (float)samples);

  Serial.print("Current Min : ");
  Serial.println(cMin);

  Serial.print("Current Max : ");
  Serial.println(cMax);

  delay(1000);
}