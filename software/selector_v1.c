#include <gtk/gtk.h>
#include <math.h>

/* Global variables */
#define GRID_SIZE 50
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

static GtkWidget *drawing_area;
// Coordinates of the box
static double box_x = 0;
static double box_y = 0;

/* Draws a selector box in the area (using Cairo API)*/
static void draw_selector(cairo_t *cr, int x, int y) {
    cairo_set_source_rgba(cr, 0.0, 0.8, 0.0, 0.5); // Green 50% transparency
    cairo_rectangle(cr, x, y, GRID_SIZE, GRID_SIZE); // Create rectangle at position (x,y) w/ GRID_SIZE dimensions 
    cairo_fill(cr); // Fill rectangle with green
}

/* Callback function for the "draw" signal */
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    draw_selector(cr, box_x, box_y); // draw selector box
    return FALSE; // Indicate draw operation is complete
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
			send_coordinates();
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // Init GTK
    
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
    return 0; // Indicate successfull program completion
}
