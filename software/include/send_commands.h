#ifndef SEND_COMMANDS_H
#define SEND_COMMANDS_H

#include <gtk/gtk.h>
#include <math.h>
#include <main.h>


/* Global variables */
#define GRID_SIZE 50
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 800 // Increased window width to accommodate the image display area
#define WINDOW_HEIGHT 500

void setup_plotter();

void activate_plotter(GObject* self,
		GParamSpec* pspec,
  gpointer user_data);

#endif
