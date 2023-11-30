#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <drawable_canvas.h>
#include <selector.h>

// Kinect support
#include <gfreenect.h>
#include <skeltrack.h>

// Window title names
#define TITLE_CANVAS "TagGang"
#define TITLE_SELECTOR "Place your tag"
#define TITLE_GCODER "Confirm"
#define TITLE_PLOTTER "Plotting!"

#define MOTOR_DISTANCE 1499 //mm, ie 59"

#define THRESHOLD_BEGIN 500
#define THRESHOLD_END 1500 

#define JOY_POLL_PERIOD 50 // ms to poll joystick for data

// Shared variables
extern GtkWidget *window;
extern GFreenectDevice *kinect;
extern SkeltrackSkeleton *skeleton;
extern volatile int joy_available;
extern volatile int btn_available;
extern volatile int joy_x, joy_y;
extern volatile struct js_event event;

// types, structs, whatever
typedef struct
{
  guint16 *reduced_buffer;
  gint width;
  gint height;
  gint reduced_width;
  gint reduced_height;
} BufferInfo;

// Mappings for buttons on a right joycon
typedef enum {
	BTN_RIGHT_B,
	BTN_RIGHT_A,
	BTN_RIGHT_X,
	BTN_RIGHT_Y,
	BTN_RIGHT_SL,
	BTN_RIGHT_R,
	BTN_RIGHT_SR,
	BTN_RIGHT_ZR,
	BTN_RIGHT_PLUS,
	BTN_RIGHT_HOME,
	BTN_RIGHT_JOY,
	BTN_RIGHT_NONE
} JoyButton;

#define JOY_RIGHT_X 0
#define JOY_RIGHT_Y 1

#define DEADZONE 4000

#define JOY_SPEED 4.0 

#endif
