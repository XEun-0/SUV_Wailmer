void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available())
    Serial.write( toupper (Serial.read()) );
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
