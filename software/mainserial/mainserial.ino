void setup(){
  Serial.begin(115200); // Start serial communication at 9600 baud
  // Serial.println("Loading TagGang firmware!");
  // setup_motors();
}

void loop(){
  if (Serial.available()) {
    Serial.print("Received");
    String cmd = Serial.readStringUntil(' ');
    Serial.print(cmd);
  }
}
