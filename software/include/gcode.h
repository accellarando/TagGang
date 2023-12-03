#ifndef GCODE_H
#define GCODE_H

#include <gtk/gtk.h>
#include <drawable_canvas.h>
#include <selector.h>
#include <main.h>
#include <math.h>

void activate_gcoder(GObject* self,
		GParamSpec* pspec,
		gpointer user_data);
		
#endif
