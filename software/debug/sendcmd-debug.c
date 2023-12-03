/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 */
//#include <send_commands.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512

static int send_command(char* cmd){
	char* python = "python serialsend.py %s";
	char buffer[BUFFER_SIZE + strlen(python)];
	sprintf(buffer, python, cmd);
	int status = system(cmd);
	return status;
}

static void parse_gcode(){
	FILE *gcode_file = fopen("/home/ella/Desktop/TagGang/software/debug/gtext.gcode", "r");
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

parse_gcode();



