/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 */
#include <send_commands.h>

/**
 * Sets up the GUI for this stage, then starts the file reading/sending process.
 */
void activate_plotter(GtkApplication *app, void* hi, gpointer data){
	// Set up GUI with some placeholder text
	
	// Read file line by line, send over serial terminal
}
