unsigned char buffer_RTT[4] = {0};
uint8_t CS;
#define COM 0x55
int Distance = 0;
bool isPinging = true; // Flag to indicate if the sensor should run
int measuredDistance = 0; // Variable to store the distance when pinger is activated

void setup() {
  Serial.begin(115200);    // Begin Serial on the USB interface
  Serial2.begin(115200);   // Begin Serial2 on pins 16 (RX) and 17 (TX)
}

void loop() {
  // Check if a command is received over Serial
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the command from Serial

    // Check if the received command is the trigger for the sensor
    if (command == 'P') { // 'P' could be a trigger for the pinger
      isPinging = true; // Set the flag to true to start reading distance
    }
  }

  // If the pinger is active, perform the distance measurement
  if (isPinging) {
    measuredDistance = measureDistance(); // Call the function to measure distance
    isPinging = false; // Reset the flag after measurement

    // Send the measured distance back to Serial
    Serial.print("Measured Distance: ");
    Serial.print(measuredDistance);
    Serial.println(" mm");
  }
}

int measureDistance() {
  // Flush incoming buffer on Serial2 to remove any unread data
  while (Serial2.available() > 0) {
    Serial2.read();  // Discard any bytes in the buffer
  }

  // Now send the command to the sensor
  Serial2.write(COM);      // Send the command over Serial2
  Serial2.flush();         // Ensure all outgoing data is transmitted
  delay(100);

  if (Serial2.available() > 0) {
    delay(4);
    
    // Check for start byte (0xFF)
    while (true) {
      if (Serial2.available() > 0) {
        if (Serial2.read() == 0xFF) {
          buffer_RTT[0] = 0xFF;

          // Read the next byte
          if (Serial2.available() > 0) {
            buffer_RTT[1] = Serial2.read();

            // Check for potential misalignment (if buffer_RTT[1] is also 0xFF)
            if (buffer_RTT[1] == 0xFF) {
              // If the second byte is also 0xFF, skip to the next header
              continue;  // Restart the loop to search for a new header
            }

            // Read the next 2 bytes
            for (int i = 2; i < 4; i++) {
              if (Serial2.available() > 0) {
                buffer_RTT[i] = Serial2.read();
              }
            }

            // Calculate the checksum
            CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];

            // Debug: Print all 4 bytes received
            Serial.print("Bytes received: ");
            for (int i = 0; i < 4; i++) {
              Serial.print("0x");
              Serial.print(buffer_RTT[i], HEX);
              Serial.print(" ");
            }

            Serial.print("CS: ");
            Serial.print(CS);
            Serial.println();

            // Check if the checksum is valid
            if (buffer_RTT[3] == CS) {
              // Valid data, process the distance
              int distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
              return distance; // Return the measured distance
            }
            break;  // Exit the while loop if we successfully processed a packet
          }
        }
      }
    }
  }
  
  return -1; // Return -1 if no valid distance is measured
}