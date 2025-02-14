
void setup() {
    Serial.begin(115200);
}

void loop() {
    // If serial data is pending, read, capitalize and write the character
    if (Serial.available())
        Serial.write( toupper (Serial.read()) );
}
