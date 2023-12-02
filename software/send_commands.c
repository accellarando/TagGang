/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 */
#include <send_commands.h>
#include <stdio.h>

#define BUFFER_SIZE 512

static int send_command(char* cmd){
	char* python = "python serialMotorCMD.py %s";
	char buffer[BUFFER_SIZE + strlen(python)];
	sprintf(buffer, python, cmd);
	int status = system(cmd);
	return status;
}

static void parse_gcode(char* filename){
	FILE *gcode_file = fopen("/home/ella/Documents/gtext.gcode", "r");
	if(gcode_file == NULL){
		printf("Error opening gcode file!\n");
	}

	char line[BUFFER_SIZE];
	while(fgets(line, sizeof(line), gcode_file) != NULL){
		int err = send_command(line);
		if(err != 0)
			printf("Error from plotter: %d\n", err);
	}

	fclose(gcode_file);
}

static GtkWidget *label;

static void finish_sending_stage() {
	// Clean up widgets
	gtk_widget_destroy(label);

	// Change window title
	gtk_window_set_title(GTK_WINDOW(window), TITLE_CANVAS);
}

/**
 * Sets up the GUI for this stage, then starts the file reading/sending process.
 */
void activate_plotter(GObject *self, GParamSpec* pspec, gpointer data){
	// Set up GUI with some placeholder text
	// TODO: put a loading bar here that lets you keep track of progress. For now, just put some placeholder text.
	label = gtk_label_new("Sending over serial!");
    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_widget_show_all(window);

	// Read file line by line, send over serial terminal
	parse_gcode("output.gcode");

	finish_sending_stage();
}



