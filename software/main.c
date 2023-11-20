/**
 * This is the TagGang main program entry.
 * It moves through three different screens:
 * 1. Drawing screen
 * 2. Tag placement screen
 * 3. Confirmation and progress, with gcode generation
 * 4. Loop back to 1, retaining state as applicable
 */
#include <main.h>

// Global/shared variables
// GtkApplication *app;
GtkWidget *window;

void scale_point_cloud(GtkApplication *app,
	gpointer data){
	GList* paths = (GList*) data;
}

void activate_gcoder(GtkApplication *app, gpointer data){
}

void activate_plotter(GtkApplication *app, gpointer data){
}

int state = 0;
void signal_router(GtkApplication *app,
		gpointer data){
	//todo: typedef enum this guy
	switch(state){
		case 0: 
			printf("Signal router hit! State: %d\n", state);
			activate_selector(app, data);
			state = 1;
			break;
		case 1:
			activate_gcoder(app, data);
			state = 2;
			break;
		case 2:
			activate_plotter(app, data);
			state = 3;
			break;
		case 3:
			activate_canvas(app, data);
			break;
		default:
			activate_canvas(app, data);
	}
}

void activate(GtkApplication *app,
		gpointer data){
	// Activate the drawing canvas with a callback to the next step
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "TagGang Main!");

	// Route signals that the child functions emit
	g_signal_connect (window, "notify::title", G_CALLBACK(signal_router), window);
	/** These don't work >:(
	g_signal_connect (window, "canvas-done", G_CALLBACK(activate_selector), window); //reads file from directory
	g_signal_connect (window, "selector-done", G_CALLBACK(activate_gcoder), points_list);
	g_signal_connect (window, "gcode-done", G_CALLBACK(activate_plotter), NULL); 
	g_signal_connect (window, "drawing_done", G_CALLBACK(activate_canvas), NULL);
	*/

	// Start with the canvas
	activate_canvas(app, window);
}

int main (int    argc,
		char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.taggang.main", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (activate), window);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
