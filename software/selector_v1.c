#include <gtk/gtk.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


/* Global variables */
#define GRID_SIZE 50
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

static GtkWidget *drawing_area;
static GList *image_list = NULL; // List to store loaded images and their coordinates
static double box_x = 0;
static double box_y = 0;
static double image_x = 0;
static double image_y = 0;
static char *image_file_path = NULL;
static GdkPixbuf *image_pixbuf = NULL;

/* Draws a selector box onto the drawing area (using Cairo API) */
static void draw_selector(cairo_t *cr, int x, int y) {
    cairo_set_source_rgba(cr, 0.0, 0.8, 0.0, 0.5); // Green 50% transparency
    cairo_rectangle(cr, x, y, GRID_SIZE, GRID_SIZE); // Create rectangle at position (x,y) w/ GRID_SIZE dimensions 
    cairo_fill(cr); // Fill rectangle with green
}

/* Draws the loaded image onto the drawing area (using Cairo API) */
static void draw_images(cairo_t *cr) {
	if(image_pixbuf != NULL) {
		gdk_cairo_set_source_pixbuf(cr, image_pixbuf, image_x, image_y);
		cairo_paint(cr);
	}
}

/* Moves the selector box in intervals within window boundaries */
static void move_box(double dx, double dy) {
	// Update box coordinates
    box_x += dx * GRID_SIZE;
    box_y += dy * GRID_SIZE;
    
    // Snap to grid
    box_x = SNAP_INTERVAL* GRID_SIZE * (int)(box_x / (SNAP_INTERVAL * GRID_SIZE));
    box_y = SNAP_INTERVAL* GRID_SIZE * (int)(box_y / (SNAP_INTERVAL * GRID_SIZE));
    
    // Boundary checks to ensure box stays within window space
    if (box_x < 0)
		box_x = 0;
		
    if (box_y < 0)
		box_y = 0;
    
    if (box_x + GRID_SIZE > WINDOW_WIDTH)
		box_x = WINDOW_WIDTH - GRID_SIZE;
		
    if (box_y + GRID_SIZE > WINDOW_HEIGHT)
		box_y = WINDOW_HEIGHT - GRID_SIZE;

    gtk_widget_queue_draw(drawing_area); // Redraw the drawing area
}

/* Callback function for the "draw" signal */
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    draw_selector(cr, box_x, box_y); // draw selector box
    draw_images(cr); // draw loaded images
    return FALSE; // Indicate draw operation is complete
}

/* !!! fix to send out to gcode CAM instead of printing */
static void send_coordinates() {
	g_print("Box Coordinates: (%.0f, %.0f)\n", box_x, box_y);
}

/* Callback function for the "key-press-event" signal */
static void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    // Handle arrow key events to move the box in snapping intervals
    switch (event->keyval) {
        case GDK_KEY_Up:
            move_box(0, -SNAP_INTERVAL);
            break;
        case GDK_KEY_Down:
            move_box(0, SNAP_INTERVAL);
            break;
        case GDK_KEY_Left:
            move_box(-SNAP_INTERVAL, 0);
            break;
        case GDK_KEY_Right:
            move_box(SNAP_INTERVAL, 0);
            break;
        case GDK_KEY_Return:
            // send_coordinates();

            image_x = box_x;
            image_y = box_y;

            if (image_file_path != NULL) {
                if (image_pixbuf != NULL) {
                    g_object_unref(image_pixbuf);
                    image_pixbuf = NULL;
                }
                GError *error = NULL;
                image_pixbuf = gdk_pixbuf_new_from_file(image_file_path, &error);

                if (error != NULL) {
                    g_printerr("Error loading %s: %s\n", image_file_path, error->message);
                    g_error_free(error);
                }

                // Shrink the image to the size of the selector box
                GdkPixbuf *shrunken_pixbuf = gdk_pixbuf_scale_simple(
                    image_pixbuf,
                    GRID_SIZE,
                    GRID_SIZE,
                    GDK_INTERP_BILINEAR
                );

                g_object_unref(image_pixbuf);
                image_pixbuf = shrunken_pixbuf;

                gtk_widget_queue_draw(drawing_area);
            }
            break;
        default:
            break;
    }
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // Init GTK
    
    if (argc != 2) {
		g_print("Error", argv[0]);
		return -1;
	}
	
	image_file_path = argv[1];
		
    // Create window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Snapping Box");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Create drawing area
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

	// Connects the signals to their respective callback event functions
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    
    gtk_widget_set_events(window, GDK_KEY_PRESS_MASK); // Enable key press events for the window
    gtk_widget_show_all(window); // Show all widgets in window

    gtk_main(); // Start GTK main loop
    
    g_list_free_full(image_list, g_free); // Free the memory allocated for the ImageInfo struct
    
    return 0; // Indicate successfull program completion
}
