#ifndef SELECTOR_H
#define SELECTOR_H

#include <gtk/gtk.h>
#include <math.h>
#include <main.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


/* Global variables */
#define GRID_SIZE 100
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 800 // Increased window width to accommodate the image display area
#define WINDOW_HEIGHT 500

extern GtkWidget *selector_area;
void activate_selector(GtkApplication* self,
		GParamSpec* pspec,
  gpointer user_data);

void setup_selector();

#endif
