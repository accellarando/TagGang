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
 *  - G28 : Home the motors
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

Stepper motorL(200, DIR_PIN_L, STEP_PIN_L);
Stepper motorR(200, DIR_PIN_R, STEP_PIN_R);

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

// measure what these need to be
#define MIN_L 00
#define MIN_R 00
#define START_L 100
#define START_R 100
int lastL = START_L; 
int lastR = START_R;
// Move to absolute position
int move_motors(double l, double r){
	if(l<MIN_L)
		return -1;
	if(r<MIN_R)
		return -2;

	int dl = l - lastL;
	int dr = r - lastR;

	// Convert l and r to motor steps
	int lSteps = dl / MM_PER_STEP;
	int rSteps = dr / MM_PER_STEP;

	// Each motor step needs 8 clock signals
	lSteps *= 8;
	rSteps *= 8;

  int lStepsDone = 0;
  int rStepsDone = 0;
  while(lStepsDone != lSteps && rStepsDone != rSteps){
      digitalWrite(ENABLE_PIN_L, HIGH);
      digitalWrite(ENABLE_PIN_R, HIGH);
      if(lStepsDone < lSteps){
        //Serial.println("Stepping L up");
        motorL.step(8);
        lStepsDone += 8;
      }
      else if(lStepsDone > lSteps){
        //Serial.println("Stepping L down");
        motorL.step(-8);
        lStepsDone -= 8;
      }
      if(rStepsDone < rSteps){
        motorR.step(8);
        rStepsDone += 8;
      }
      else if(rStepsDone > rSteps){
        motorR.step(-8);
        rStepsDone -= 8;
      }
      //motorR.step(8);
      digitalWrite(ENABLE_PIN_L, LOW);
      digitalWrite(ENABLE_PIN_R, LOW);
  }
	// Move motors
	// TODO: some sort of safety here to make sure neither goes negative?
	//motorL.moveTo(lSteps); // these are absolute positions, not relative
	//motorR.moveTo(rSteps);


	lastL += dl;
	lastR += dr;
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
          Serial.println(point);
					while(cmd[strPos] != ' ' && cmd[strPos] != '\n'){
						point[i] = cmd[strPos];
						strPos++;
						i++;
					}
          Serial.println(point);
					points[j] = String(point).toDouble();
          Serial.println(points[j], 1);
					strPos += 2; //skip the space and letter if applicable
				}
        //Serial.println("points: %f %f", points[0], points[1]);
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

	digitalWrite(CONTROL_PIN_L, LOW);
	digitalWrite(CONTROL_PIN_R, LOW);

	motorL.setSpeed(100); // may need to change this param
	motorR.setSpeed(100); // may need to change this param
}

/**
 * Set up motors and pins, then send OK when read
*/
int i;
void setup(){
	Serial.begin(115200); // Start serial communication at 9600 baud
	//Serial.println("Loading TagGang firmware!");

	//setup_motors();

	//Serial.println("OK");
 //i = 0;
}

void loop(){
  if (Serial.available()){
    String cmd = Serial.readStringUntil('\n'); // quirk: the string has to have a space before \n for some reason?
    Serial.print(cmd); // for debug
  
  }
  else{
    Serial.println("No serial data available");
  }
	
//	int err = exec_command(cmd);
//  int err = 0;
//  move_motors(50+i++, 50+i++);
//  delay(5000);
//	if(!err){
//		Serial.println("OK");
//	}
//	else{
//		char response[4];
//		sprintf(response, "E%03d", err);
//		Serial.println(response);
//	}
}
