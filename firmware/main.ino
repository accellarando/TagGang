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
 *  - G28 : Home the motors
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
#include <stdio.h>

typedef enum{
	CMD_G1XY,
	CMD_G1Z,
	CMD_G28
} CommandType;

int move_to_point(double x, double y){
	return 1;
}

#define DRY_RUN 1 
int send_command(CommandType cmdType, double xz, double y){
#if defined(DRY_RUN) && !DRY_RUN
	char buffer[10];
	sprintf(buffer, "%d %f %f", cmdType, xz, y);
	Serial.println(buffer);
	return 0;
#else
	switch(cmdType){
		case CMD_G1XY:
			return move_to_point(xz, y);
			break;
		case CMD_G1Z:
			if(xz == 0.0){
				return pen_down();
			}
			else{
				return pen_up();
			}
			break;
		default:
			return 1;
	}
	return 0;
#endif
}

/**
 * this little guy is kind of quirky. i was crossfaded when i wrote it but i
 * think it'll get the job done.
 *
 * Each G1XY coordinate must be in the order X%f Y%f
 * and the Y coordinate must be terminated with a \n or a space.
 * 
 * Supports the commands listed in the file's top comment. go read that one.
 */
int exec_command(char* cmd){
	int cmdType = 0;
	int points[2];
	if(cmd[0] != 'G'){
		return 1; // not supported
	}
	switch(cmd[1]){
		case '1':
			if(cmd[3] == 'X'){
				// get parsin
				int strPos = 4;
				for(int j=0; j<2; j++){
					char point[10];
					while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
						point[i] = cmd[strPos];
						strPos++;
					}
					point[j] = atoi(point);
					strPos++; //skip the space if applicable
				}
				return send_command(CMD_G1XY, point[0], point[1]);
			}
			else if(cmd[3] == 'Z'){
				char amt[10];
				int strPos = 4;
				while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
					amt[i] = cmd[strPos];
					strPos++;
				}
				return send_command(CMD_G1Z, atoi(amt), -1);
			}
			else{
				return 1; // not supported
			}
		break;
		case '2':
			if(cmd[2] == '8')
				return send_command(CMD_G28, -1, -1);
			else
				return 1; //not supported
		default:
			return 1; // not supported
	}
	return 0;
}

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

	Serial.println("OK");
}

void loop(){
	char* cmd = Serial.readStringUntil('\n');
	int err = exec_command(cmd);
	if(!err){
		Serial.println("OK");
	}
	else{
		char response[4];
		sprintf(response, "E%03d", err);
		Serial.println(response);
	}
}
