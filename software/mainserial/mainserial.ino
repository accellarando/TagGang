void setup(){
  Serial.begin(9600); // Start serial communication at 9600 baud
  // Serial.println("Loading TagGang firmware!");
  // setup_motors();
}

void loop(){
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    Serial.print(cmd);
  }
}
