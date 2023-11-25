/*
 * Arduino code for TagGang vertical pen plotter.
 *
 * This code takes input from the serial port to control the plotter.
 *
 * This input is a \n-terminated string that implements a couple of gcode
 * commands.
 *
 * The commands are:
 *  - G1 X<pos> Y<pos> : Move to position <pos>. Units tbd
 *  - G1 Z<pos> : Move pen down onto the canvas, which is at 0 units
 *
 * TODO: 
 *  - Read input from serial terminal
 *  - Cartesian coordinates to motor movements
 *  - Implement G1XY
 *  - Implement G1Z
 *  - Implement homing procedure
 *
 * References much of the source code from https://github.com/euphy/polargraph_server_a1/.
 */

#include <pins.h>

void setup_motors(){
	// sexy sexy pin configuration
	pinMode(PIN_L_NFAULT, INPUT);
	pinMode(PIN_L_STEP, OUTPUT);
	pinMode(PIN_L_DIR, OUTPUT);
	pinMode(PIN_L_NENBL, OUTPUT);
	pinMode(PIN_L_NHOME, INPUT);

	pinMode(PIN_R_NFAULT, INPUT);
	pinMode(PIN_R_STEP, OUTPUT);
	pinMode(PIN_R_DIR, OUTPUT);
	pinMode(PIN_R_NENBL, OUTPUT);
	pinMode(PIN_R_NHOME, INPUT);

	// Initialize stepper motors
	digitalWrite(PIN_L_NENBL, LOW);
	digitalWrite(PIN_R_NENBL, LOW);

	AccelStepper motorL(AccelStepper::FULL4WIRE, PIN_L_STEP, PIN_L_DIR);
	AccelStepper motorR(AccelStepper::FULL4WIRE, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN); 
}

void setup(){
	Serial.begin(9600); // Start serial communication at 9600 baud
	Serial.println("Loading TagGang firmware!");

    setup_motors();

}

void loop(){
	char* cmd = Serial.readStringUntil('\n');
	int err = exec_command(cmd);
	if(!err)
		Serial.println("OK");
	else
		Serial.println(err);
}
