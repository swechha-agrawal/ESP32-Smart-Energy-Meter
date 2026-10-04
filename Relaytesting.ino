int relayPin = 32; // your GPIO
void setup() {
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, HIGH); // turn ON
  delay(1000);
  digitalWrite(relayPin, LOW); // turn OFF
  delay(1000);
}

 





