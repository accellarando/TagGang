#ifndef GCODE_H
#define GCODE_H

#include <gtk/gtk.h>
#include <drawable_canvas.h>
#include <selector.h>
#include <main.h>
#include <math.h>

// Canvas setup, all in mm
#define CUTOFF_TOP 371
#define CUTOFF_BOTTOM 1081
#define CUTOFF_LEFT 60
#define CUTOFF_RIGHT 60

#define MOTOR_DISTANCE 610

#define CANVAS_WIDTH (MOTOR_DISTANCE - CUTOFF_LEFT - CUTOFF_RIGHT)
#define CANVAS_HEIGHT (CUTOFF_BOTTOM - CUTOFF_TOP)

void setup_gcoder();

void activate_gcoder(GObject* self,
		GParamSpec* pspec,
		gpointer user_data);
		
#endif
