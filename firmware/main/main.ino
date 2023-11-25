/*
 * Arduino code for TagGang vertical pen plotter.
 *
 * This code takes input from the serial port to control the plotter.
 *
 * This input is a \n-terminated string that implements a couple of gcode
 * commands.
 *
 * The commands are:
 *  - G1 L<pos> R<pos> : Move to position <pos>. Units tbd
 *  - G1 Z<pos> : Move pen down onto or up off the canvas, which is at 0 units
 *  - G28 : Home the motors
 *
 * TODO: 
 *  - Implement homing procedure (G28)
 *  - Bounds checking on requested coordinates (ie no negative coordinates please)
 *  - Figure out dimensions of canvas, belt, homing position, etc
 *
 * References some source code from https://github.com/euphy/polargraph_server_a1/.
 */
#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#include "pins.h"

typedef enum{
	CMD_G1LR,
	CMD_G1Z,
	CMD_G28
} CommandType;

Servo penServo;

  AccelStepper motorL(AccelStepper::FULL4WIRE, PIN_L_STEP, PIN_L_DIR);
  AccelStepper motorR(AccelStepper::FULL4WIRE, PIN_L_STEP, PIN_R_STEP); 
  
int pen_down(){
	penServo.attach(PIN_PEN_SERVO);
	for(int i=0; i<180; i++){
		penServo.write(i);
		delay(15); // i guess?
	}
	penServo.detach();
}

int pen_up(){
	penServo.attach(PIN_PEN_SERVO);
	for(int i=180; i>0; i--){
		penServo.write(i);
		delay(15); 
	}
	penServo.detach();
}

#define MM_PER_STEP 0.1 // read belt datasheet?
int move_motors(double l, double r){
	// Convert l and r to motor steps
	int lSteps = l / MM_PER_STEP;
	int rSteps = r / MM_PER_STEP;

	// Move motors
	// TODO: some sort of safety here to make sure neither goes negative?
	motorL.moveTo(lSteps); // these are absolute positions, not relative
	motorR.moveTo(rSteps);
}

#define DRY_RUN 1 
int send_command(CommandType cmdType, double lz, double r){
#if defined(DRY_RUN) && !DRY_RUN
	char buffer[10];
	sprintf(buffer, "%d %f %f", cmdType, lz, r);
	Serial.println(buffer);
	return 0;
#else
	switch(cmdType){
		case CMD_G1LR:
			return move_motors(lz, r);
			break;
		case CMD_G1Z:
			if(lz == 0.0){
				return pen_down();
			}
			else{
				return pen_up();
			}
			break;
		default:
			return 5;
	}
	return 0;
#endif
}

/**
 * this little guy is kind of quirky. i was crossfaded when i wrote it but i
 * think it'll get the job done.
 *
 * Each G1LR coordinate must be in the order X%f Y%f
 * and the Y coordinate must be terminated with a \n or a space.
 * 
 * Supports the commands listed in the file's top comment. go read that one.
 */
int exec_command(String cmd){
	int cmdType = 0;
	int points[2];
	if(cmd[0] != 'G'){
		return 1; // not supported
	}
  Serial.println(cmd[1] == '1');
	switch(cmd[1]){
		case '1':
			if(cmd[3] == 'L'){
				// get parsin
				int strPos = 4;
        int i = 0;
				for(int j=0; j<2; j++){
					char point[10];
					while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
						point[i] = cmd[strPos];
						strPos++;
           i++;
					}
					points[j] = atoi(point);
					strPos += 2; //skip the space and letter if applicable
				}
				return send_command(CMD_G1LR, points[0], points[1]);
			}
			else if(cmd[3] == 'Z'){
				char amt[10];
				int strPos = 4;
        int i = 0;
				while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
					amt[i] = cmd[strPos];
					strPos++;
          i++;
				}
				return send_command(CMD_G1Z, atoi(amt), -1);
			}
			else{
				return 2; // not supported
			}
		break;
		case '2':
			if(cmd[2] == '8')
				return send_command(CMD_G28, -1, -1);
			else
				return 3; //not supported
		default:
			return 4; // not supported
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

}

void setup(){
	Serial.begin(9600); // Start serial communication at 9600 baud
	Serial.println("Loading TagGang firmware!");

    setup_motors();

	Serial.println("OK");
}

void loop(){
	String cmd = Serial.readStringUntil('\n');
  Serial.println(cmd);
	int err = exec_command(cmd);
	if(!err){
		Serial.println("OK");
	}
	else{
		char response[4];
		sprintf(response, "E%03d", err);
		Serial.println(response);
	}
	while(motorL.distanceToGo() != 0 || motorR.distanceToGo() != 0){
		motorL.run();
		motorR.run();
	}
}
