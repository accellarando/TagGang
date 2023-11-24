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

void activate_plotter(GtkApplication *app, void* hi, gpointer data){
}


/**
 * This function routes Title Change signals to the appropriate
 * function, based on the current state of the program.
 */
void signal_router(GtkApplication *app,
		gpointer data){
	if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_SELECTOR) == 0){
		activate_selector(app, NULL, data);
	}
	else if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_GCODER) == 0){
		activate_gcoder(app, NULL, data);
	}
	else if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_PLOTTER) == 0){
		activate_plotter(app, NULL, data);
	}
	else {
		activate_canvas(app, NULL, data);
	}
}

void activate(GtkApplication *app,
		gpointer data){
	// Activate the drawing canvas with a callback to the next step
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Draw a picture!");
    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Route signals that the child functions emit
	g_signal_connect (window, "notify::title", G_CALLBACK(signal_router), app);

	// Start with the canvas
	activate_canvas(app, NULL, window);
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
