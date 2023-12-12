#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Stepper.h>

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
 *  - G28 : Home the motors9
 *
 * TODO: 
 *  - Implement homing procedure (G28)
 *  - Bounds checking on requested coordinates (ie no negative coordinates please)
 *  	- Figure out dimensions of canvas, belt, homing position, etc
 *
 * References some source code from https://github.com/euphy/polargraph_server_a1/.
 */
#include <Arduino.h>
#include <Servo.h>

#include "pins.h"

/**
 * Enum that lists the different command types supported by the firmware.
 */
typedef enum{
	CMD_G1LR,
	CMD_G1Z,
	CMD_G28
} CommandType;

Servo penServo;

AccelStepper motorL(AccelStepper::DRIVER, STEP_PIN_L, DIR_PIN_L);
AccelStepper motorR(AccelStepper::DRIVER, STEP_PIN_R, DIR_PIN_R);

// Use the MultiStepper library to control these guys
MultiStepper plotter;

/**
 * Directions or angles may be wrong in either of these functions.
 */
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

// These have been measured as a viable homing position
#define START_L STEPS_L(-480)
#define START_R STEPS_R(480)
double lastL = START_L; 
double lastR = START_R;
// Move to absolute position
int move_motors(double l, double r){
	long position[2] = {(long)(STEPS_L(-l)), (long)(STEPS_R(r))};
	plotter.moveTo(position);
  digitalWrite(ENABLE_PIN_L, HIGH);
  digitalWrite(ENABLE_PIN_R, HIGH);
  plotter.runSpeedToPosition();
  digitalWrite(ENABLE_PIN_L, LOW);
  digitalWrite(ENABLE_PIN_R, LOW);

	return 0;
}

/**
 * Comment out the DRY_RUN macro to actually send commands to motors.
 * Otherwise, it will print out the command contents to the serial
 * terminal for debugging.
 */
//#define DRY_RUN 1 
int send_command(CommandType cmdType, double lz, double r){
#if defined(DRY_RUN) && DRY_RUN
	char buffer[10];
	sprintf(buffer, "%d %f %f", cmdType, lz, r);
	Serial.println(buffer);
	Serial.println(lz, 2);
	Serial.println(r, 2);
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
			//TODO: implement G28 (homing function)
		case CMD_G28:
			return move_motors(START_L, START_R);
		default:
			return 5;
	}
	return 0;
#endif
}

/**
 * Each G1LR coordinate must be in the order L%f R%f
 * and the R coordinate must be terminated with a \n or a space.
 * 
 * Supports the commands listed in the file's top comment. go read that one.
 */
int exec_command(String cmd){
	int cmdType = 0;
	double points[2];
	if(cmd[0] != 'G'){
		return 1; // not supported
	}
	//Serial.println(cmd[1] == '1');
	switch(cmd[1]){
		case '1':
			if(cmd[3] == 'L'){
				// get parsin
				int strPos = 4;
				int i = 0;
				for(int j=0; j<2; j++){
					char point[10];
					memset(point, 0, sizeof(point));
					i = 0;
					//Serial.println(point);
					while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
						point[i] = cmd[strPos];
						strPos++;
						i++;
					}
					points[j] = String(point).toDouble();
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
	pinMode(DIR_PIN_L, OUTPUT);
	pinMode(DIR_PIN_R, OUTPUT);
	pinMode(STEP_PIN_L, OUTPUT);
	pinMode(STEP_PIN_R, OUTPUT);
	pinMode(ENABLE_PIN_L, OUTPUT);
	pinMode(ENABLE_PIN_R, OUTPUT);
	pinMode(CONTROL_PIN_L, OUTPUT);
	pinMode(CONTROL_PIN_R, OUTPUT);
	pinMode(RESET_PIN_L, OUTPUT);
	pinMode(RESET_PIN_R, OUTPUT);

	// Initialize stepper motors
	analogWrite(VREFA_PIN_L, 50);
	analogWrite(VREFB_PIN_L, 50);
	analogWrite(VREFA_PIN_R, 50);
	analogWrite(VREFB_PIN_R, 50);

	digitalWrite(RESET_PIN_L, HIGH);
	digitalWrite(RESET_PIN_R, HIGH);

	digitalWrite(CONTROL_PIN_L, HIGH);
	digitalWrite(CONTROL_PIN_R, HIGH);

  motorL.setMaxSpeed(100);
  motorR.setMaxSpeed(100);
  motorL.setCurrentPosition(START_L);
  motorR.setCurrentPosition(START_R);
  motorL.setAcceleration(200);
  motorR.setAcceleration(200);
  //motorL.setEnablePin(ENABLE_PIN_L);
  //motorR.setEnablePin(ENABLE_PIN_R);
	plotter.addStepper(motorL);
	plotter.addStepper(motorR);
}

/**
 * Set up motors and pins, then send OK when read
 */
void setup(){
	Serial.begin(9600); // Start serial communication at 9600 baud
	Serial.println("Loading TagGang firmware!");

	setup_motors();

	//Serial.println("OK");
}

void loop(){
	if (Serial.available()){
		String cmd = Serial.readStringUntil('\n'); // quirk: the string has to have a space before \n for some reason?
		//Serial.print(cmd); // for debug
	

	int err = exec_command(cmd);
	//  int err = 0;
	//  move_motors(50+i++, 50+i++);
	//  delay(5000);
	if(!err){
		Serial.println("OK");
	}
	else{
		char response[4];
		sprintf(response, "E%03d", err);
		Serial.println(response);
	}
	}
}
