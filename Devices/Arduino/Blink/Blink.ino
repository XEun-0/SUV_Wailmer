void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available())
    Serial.write( toupper (Serial.read()) );
}
