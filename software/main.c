/**
 * This is the TagGang main program entry.
 * It moves through three different screens:
 * 1. Drawing screen
 * 2. Tag placement screen
 * 3. Confirmation and progress
 * 4. GOTO 1
 */

#include <gtk/gtk.h>
#include <drawable_canvas.h>

void activate(GtkApplication *app,
		gpointer data){
	// Activate the drawing canvas with a callback to the next step
	activate_canvas(app, data);
}

int main (int    argc,
		char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.taggang.main", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
