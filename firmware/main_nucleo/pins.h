// Pins
#define STEP_PIN_L		PB2 // PD5 net on board
#define DIR_PIN_L		  PB4 // PB0 net on board
#define ENABLE_PIN_L	PD5 // PD7 net on board

// These pins are on the NUCLEO driver
#define STEP_PIN_R		PD6
#define DIR_PIN_R		  PD7
#define ENABLE_PIN_R	PD2
#define CTR_PIN       PD5
#define RST_PIN       PB0
#define REFA_PIN      PD3
#define REFB_PIN      PB1

#define PIN_PEN_SERVO A2

// Stepper motor constants
#define GEAR_RADIUS 4.0 //mm
#define DEGREES_PER_STEP 1.8
#define PULSES_PER_STEP 1.0
#define STEPS_PER_MM ((180.0*PULSES_PER_STEP) / (PI * GEAR_RADIUS * DEGREES_PER_STEP))
#define STEPS(x) (x*STEPS_PER_MM)
