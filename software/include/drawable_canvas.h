#ifndef DRAWABLE_CANVAS_H
#define DRAWABLE_CANVAS_H

#include <gtk/gtk.h>
#include <math.h>
#include <main.h>

// Point structure
typedef struct {
	gdouble x;
	gdouble y;
} DoublePoint;

void activate_canvas (GtkApplication* app, void* i_forgot, gpointer user_data);

extern GList* points_list;
#endif
