#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <drawable_canvas.h>

typedef void (*StageActivator)(GtkApplication *app,
		gpointer user_data,
		StageActivator* next_stage);

#endif
