/**
 * This file contains code to generate G-Code from a list of points.
 *
 * TODO:
 *		- Loading bar? (stretch goal)
 *		- Testing
 *		- Figure out x_scale and y_scale for scale_paths, or implement a different strategy
 *
 * @author Ella Moss
 */

#include <gcode.h>

void paths_to_gcode_file_cartesian(GList* points, char* filename){
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	GList* current_path = points;
	// skip first path, it contains the starting coordinate for scaling
	current_path = current_path->next;
	while (current_path != NULL) {
		GList* current_point = ((GList*) current_path->data);
		Point* start_point = (Point*) current_point->data;
		// Move pen down 
		fprintf(file, "G1 Z%f\n", 0); 
		current_point = current_point->next;
		while (current_point != NULL) {
			Point* point = (Point*) current_point->data;
			fprintf(file, "G1 X%f Y%f\n", point->x, point->y);
			current_point = current_point->next;
		}
		// Move pen up
		fprintf(file, "G1 Z%f\n", 10);
		current_path = current_path->next;
	}

	fclose(file);
}

void scale_paths(GList* points, double x_factor, double y_factor) {
	GList* current = points;
	// First path contains the starting coordinate
	GList *first_path = ((GList*) current->data);
	gdouble start_x = ((Point*) first_path->data)->x;
	gdouble start_y = ((Point*) first_path->data)->y;
	current = current->next;

	while (current != NULL) {
		GList *this_path = ((GList*) current->data);
		while(this_path != NULL){
			Point* point = (Point*) this_path->data;
			point->x /= x_factor;
			point->x += start_x;
			point->y /= y_factor;
			point->y += start_y;
			this_path = this_path->next;
		}
	}
}


void activate_gcoder(GObject* self,
		GParamSpec* property, gpointer data) {

	// todo: put a loading bar here that lets you keep track of progress
	scale_paths(points_list, 10, 10);

	paths_to_gcode_file_cartesian(points_list, "output.gcode");

	finish_stage();
}
