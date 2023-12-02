/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 */
#include <send_commands.h>
#include <stdio.h>

/**
 * Sets up the GUI for this stage, then starts the file reading/sending process.
 */
void activate_plotter(GtkApplication *app, void* hi, gpointer data){
	// Set up GUI with some placeholder text
	// TODO: put a loading bar here that lets you keep track of progress. For now, just put some placeholder text.
	GtkWidget *label = gtk_label_new("Sending over serial!");
    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_widget_show_all(window);

	parse_gcode();


	finish_sending_stage();
	// Read file line by line, send over serial terminal
}



static void finish_sending_stage() {
	// Clean up widgets
	gtk_widget_destroy(drawing_area);
	gtk_widget_destroy(image_display_area);
	gtk_widget_destroy(vbox);

	// Change window title
	gtk_window_set_title(GTK_WINDOW(window), TITLE_CANVAS);
}
