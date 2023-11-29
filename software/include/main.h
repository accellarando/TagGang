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

#define MOTOR_DISTANCE 60

#define THRESHOLD_BEGIN 4 // put the actual value here
#define THRESHOLD_END 10 // put the actual value here

// Shared variables
extern GtkWidget *window;
extern GFreenectDevice *kinect;
extern SkeltrackSkeleton *skeleton;

// types, structs, whatever
typedef struct
{
  guint16 *reduced_buffer;
  gint width;
  gint height;
  gint reduced_width;
  gint reduced_height;
} BufferInfo;


#endif
