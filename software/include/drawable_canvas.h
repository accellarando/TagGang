#ifndef DRAWABLE_CANVAS_H
#define DRAWABLE_CANVAS_H

#include <gtk/gtk.h>
#include <math.h>

// Point structure
typedef struct {
	gdouble x;
	gdouble y;
} DoublePoint;

void activate_canvas (GtkApplication* app, gpointer user_data);

extern GList* points_list;
#endif
