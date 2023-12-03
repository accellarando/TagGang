void setup(){
  Serial.begin(9600); // Start serial communication at 9600 baud
  // Serial.println("Loading TagGang firmware!");
  // setup_motors();
}

void loop(){
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    //cmd.trim();
    //int cmd = Serial.readString().toInt();
    Serial.print(cmd);
    //Serial.print(cmd+1);
  }
}
