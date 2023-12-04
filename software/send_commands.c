/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 * 
 * TODO:
 * 		- loading bar that lets you keep track of progress
 * 		- fix summary
 * 
 * @author Dana Escandor
 */
#include <send_commands.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256 // buffer size for storing command strings
#define GCODE_FILE_PATH "/home/ella/Desktop/TagGang/software/output.gcode"
#define PYTHON_SCRIPT_PATH "/home/ella/Desktop/TagGang/software/serialsend.py"

static GtkWidget *label; // ? static

/**
 * @brief sends a gcode command via a python script that handles serial read/write
 * @param gcode command to be sent
 * @return status of the command execution
 */ 
static int send_command(const char* gcode_cmd){
	// constructs the command to call python script
	char python_cmd[BUFFER_SIZE];
	snprintf(python_cmd, sizeof(python_cmd),
	 "python3 %s \"%s\"", PYTHON_SCRIPT_PATH, gcode_cmd); // python3 vs python?
	
	int status = system(python_cmd);
	return status;
}

/**
 * @brief parses gcode file and sends gcode commands one line at a time 
 * to "send_command", printing any errors if any
 */ 
static void parse_gcode(){
	// reads gcode file
	FILE *gcode_file = fopen(GCODE_FILE_PATH, "r");
	if(gcode_file == NULL){
		printf("Error opening gcode file!\n");
		return;
	}

	char line[BUFFER_SIZE]; // array to store a line read from the gcode file
	// reads lines from gcode file until the end is reached
	while(fgets(line, sizeof(line), gcode_file) != NULL){
		line[strcspn(line, "\n")] = '\0'; // removes newline char before sending
		
		int err = send_command(line); // send parsed gcode command to send_command
		if(err != 0)
			printf("Error from plotter: %d\n", err);
	}

	fclose(gcode_file);
}

/**
 * @brief clears GUI and resets title
 */
static void finish_sending_stage() {
	// clean up widgets
	//gtk_widget_destroy(label);
	gtk_widget_hide(label);
	
	// change window title to loop back to beginning
	gtk_window_set_title(GTK_WINDOW(window), TITLE_CANVAS);
}

void setup_plotter(){
	// set up GUI with some placeholder text
	label = gtk_label_new("Sending over serial!");
    //gtk_container_add(GTK_CONTAINER(frame), label);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
	gtk_widget_hide(label);
}

/**
 * @brief sets up the GUI for gcode sender stage, then starts the file reading and parsing
 */
void activate_plotter(GObject *self, GParamSpec* pspec, gpointer data){
	gtk_widget_show(label);

	// read file line by line, send over serial terminal
	parse_gcode();
	
	// reset GUI
	finish_sending_stage();
}



