//////////////////////////////////////////////////////////////////////////////
//  ____  _   ___     __         __        __    _ _                        //
// / ___|| | | \ \   / /         \ \      / /_ _(_) |_ __ ___   ___ _ __    //
// \___ \| | | |\ \ / /           \ \ /\ / / _` | | | '_ ` _ \ / _ \ '__|   //
//  ___) | |_| | \ V /             \ V  V / (_| | | | | | | | |  __/ |      //
// |____/ \___/   \_/     _____     \_/\_/ \__,_|_|_|_| |_| |_|\___|_|      //
//                       |_____|                                            //
// Series: Testing                                                          //
// Platform/Device: Arduino                                                 //
//////////////////////////////////////////////////////////////////////////////

// Define the LED pin
#define LED 13

// The setup function runs once when you press reset or power the board
void setup() {
  // Initialize digital pin LED as an output
  pinMode(LED, OUTPUT);
}

// The loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH); // Turn the LED on (HIGH is the voltage level)
  delay(1000);             // Wait for a second
  digitalWrite(LED, LOW);  // Turn the LED off by making the voltage LOW
  delay(1000);             // Wait for a second
}
