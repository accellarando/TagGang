/**
 * This file contains code to generate G-Code from a list of points.
 *
 * TODO:
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

void paths_to_gcode_file(GList* points, char* filename){
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
		if(current_point == NULL){
			current_path = current_path->next;
			continue;
		}
		DoublePoint* start_point = (DoublePoint*) current_point->data;
		// Move pen to start point
		// These were already transformed in transform_paths
		fprintf(file, "G1 L%f R%f\n", start_point->x, start_point->y);
		// Move pen down 
		fprintf(file, "G1 Z%f\n", 0.0); 
		current_point = current_point->next;
		while (current_point != NULL) {
			DoublePoint* point = (DoublePoint*) current_point->data;
			fprintf(file, "G1 L%f R%f\n", point->x, point->y);
			current_point = current_point->next;
		}
		// Move pen up
		fprintf(file, "G1 Z%f\n", 180.0); // maybe in the future we want to take a degree parameter
										  // for the servo angle?
		current_path = current_path->next;
	}

	fclose(file);
}

void scale_paths(GList* points, double x_factor, double y_factor) {
	GList* current = points;
	// First path contains the starting coordinate
	GList *first_path = ((GList*) current->data);
	gdouble start_x = ((DoublePoint*) (first_path->data))->x;
	gdouble start_y = ((DoublePoint*) (first_path->data))->y;
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

void transform_paths(GList* points, double motor_distance){
	GList* current = points;
	while(current != NULL){
		GList *this_path = ((GList*) current->data);
		while(this_path != NULL){
			DoublePoint* point = (DoublePoint*) this_path->data;
			gdouble original_x = point->x;
			gdouble original_y = point->y;
			point->x = sqrt(pow(original_x,2) + pow(original_y,2));
			point->y = sqrt(pow((motor_distance - original_x),2) + pow(original_y,2));
			this_path = this_path->next;
		}
		current = current->next;
	}
}

static GtkWidget *label;

void finish_stage() {
	//gtk_widget_destroy(label);
	gtk_widget_hide(label);

	gtk_window_set_title(GTK_WINDOW(window), TITLE_PLOTTER);
}

void setup_gcoder() {
	// todo: put a loading bar here that lets you keep track of progress. For now, just put some placeholder text.
	label = gtk_label_new("Generating gcode!");
    gtk_container_add(GTK_CONTAINER(frame), label);
	gtk_widget_hide(label);
}

void activate_gcoder(GObject* self,
		GParamSpec* property, gpointer data) {
	gtk_widget_show(label);

	scale_paths(points_list, 10, 10);

	transform_paths(points_list, MOTOR_DISTANCE);

	paths_to_gcode_file(points_list, "output.gcode");

	finish_stage();
}
