#ifndef SELECTOR_H
#define SELECTOR_H

#include <gtk/gtk.h>
#include <math.h>
#include <main.h>


/* Global variables */
#define GRID_SIZE 50
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 800 // Increased window width to accommodate the image display area
#define WINDOW_HEIGHT 500

void activate_selector(GObject* self,
		GParamSpec* pspec,
  gpointer user_data);

#endif
