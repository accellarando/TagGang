/**
 * This file contains code to generate G-Code from a list of points.
 *
 * TODO:
 * 		- Coordinate transformation
 *		- Testing
 *		- Figure out x_scale and y_scale for scale_paths, or implement a different strategy
 *		- Loading bar? (stretch goal)
 *
 * @author Ella Moss
 */

/**
 * points_list is in this format:
 * { {{box_corner_X, box_corner_Y}},
 *   { {path_point_1_X, path_point_1_Y},
 *     ...
 *   },
 *   { {path_point_X, path_point_Y},
 *   ...
 *   }
 * }
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
		DoublePoint* start_point = (DoublePoint*) current_point->data;
		// Move pen to start point
		fprintf(file, "G1 X%f Y%f\n", start_point->x, start_point->y);
		// Move pen down 
		fprintf(file, "G1 Z%f\n", 0.0); 
		current_point = current_point->next;
		while (current_point != NULL) {
			DoublePoint* point = (DoublePoint*) current_point->data;
			fprintf(file, "G1 X%f Y%f\n", point->x, point->y);
			current_point = current_point->next;
		}
		// Move pen up
		fprintf(file, "G1 Z%f\n", 10.0);
		current_path = current_path->next;
	}

	fclose(file);
}

void scale_paths(GList* points, double x_factor, double y_factor) {
	GList* current = points;
	// First path contains the starting coordinate
	GList *first_path = ((GList*) current->data);
	gdouble start_x = ((DoublePoint*) first_path)->x;
	gdouble start_y = ((DoublePoint*) first_path)->y;
	current = current->next;

	while (current != NULL) {
		GList *this_path = ((GList*) current->data);
		while(this_path != NULL){
			DoublePoint* point = (DoublePoint*) this_path->data;
			point->x /= x_factor;
			point->x += start_x;
			point->y /= y_factor;
			point->y += start_y;
			this_path = this_path->next;
		}
		current = current->next;
	}
}


void activate_gcoder(GObject* self,
		GParamSpec* property, gpointer data) {

	// todo: put a loading bar here that lets you keep track of progress. For now, just put some placeholder text.
	GtkWidget *label = gtk_label_new("Generating gcode!");
    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_widget_show_all(window);

	scale_paths(points_list, 10, 10);

	paths_to_gcode_file_cartesian(points_list, "output.gcode");

	//finish_stage();
}
