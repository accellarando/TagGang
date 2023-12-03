/**
 * send_commands:
 *
 * Read output.gcode, sending one line at a time (terminated with \n).
 * After a line is sent, wait for an "OK" value coming back, or an "Exxx" error code.
 * Once EOF is reached, change title back to the TITLE_CANVAS.
 */
//#include <send_commands.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512 //256
#define GCODE_FILE_PATH "/home/ella/Desktop/TagGang/software/debug/gtext.gcode"
#define PYTHON_SCRIPT_PATH "/home/ella/Desktop/TagGang/software/serialsend.py"

static int send_command(char* cmd){
	// constructs the command to call py script
	char python_cmd[BUFFER_SIZE];
	int chars_written = snprintf(python_cmd, sizeof(python_cmd), "python3 %s \"%s\"", PYTHON_SCRIPT_PATH, cmd);
	 
	if (chars_written < 0 || chars_written >= sizeof(python_cmd)) {
		fprintf(stderr, "HELP ERROR\n");
		exit(EXIT_FAILURE);
	}
	//char* python = "python3 %s \"%s\"";
	//char buffer[BUFFER_SIZE + strlen(python)];
	//sprintf(buffer, python, PYTHON_SCRIPT_PATH, cmd);
	printf("%s_", python_cmd);
	
	//system(python_cmd);

}

static void parse_gcode(){
	FILE *gcode_file = fopen(GCODE_FILE_PATH, "r");
	if(gcode_file == NULL){
		printf("Error opening gcode file!\n");
		//exit(EXIT_FAILURE);
	}

	char line[256];
	while(fgets(line, sizeof(line), gcode_file) != NULL){
		line[strcspn(line, "\n")] = '\0'; // remove newline char
		//printf("%s_END_", line);
		send_command(line);
		//int err = send_command(line);
		//if(err != 0)
			//printf("Error from plotter: %d\n", err);
	}

	fclose(gcode_file);
}

int main() {
	parse_gcode();
	return 0;
}



