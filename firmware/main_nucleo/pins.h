// Pins
#define STEP_PIN_L		6
#define DIR_PIN_L		7
#define ENABLE_PIN_L	2
#define CONTROL_PIN_L	5
#define RESET_PIN_L		12

#define STEP_PIN_R		A0
#define DIR_PIN_R		4
#define ENABLE_PIN_R	8
#define CONTROL_PIN_R	A1
#define RESET_PIN_R		13

#define VREFA_PIN_L		3
#define VREFB_PIN_L		9
#define VREFA_PIN_R     10
#define VREFB_PIN_R		11

#define PIN_PEN_SERVO A2

// Stepper motor constants
#define GEAR_RADIUS 8.0 //mm
#define DEGREES_PER_STEP 1.8
#define MM_PER_STEP ((PI * GEAR_RADIUS * DEGREES_PER_STEP) / 180.0)
