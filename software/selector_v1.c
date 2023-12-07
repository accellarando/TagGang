/*
 * This part of the TagGang program allows users to select where on the canvas their
 * tag will be drawn.
 *
 * TODO:
 *		- Joycon drift calibration
 *
 * @author Dana Escandor
 */

#include <selector.h>
#include <stdio.h>

GtkWidget *selector_area;
static GtkWidget *image_display_area; // GtkImage widget for displaying the loaded image
static double box_x = 0;
static double box_y = 0;
static double image_x = 0;
static double image_y = 0;
static char *image_file_path = "drawing.png"; //hardcoded for now
static GdkPixbuf *image_pixbuf = NULL;
static GdkPixbuf *preview_pixbuf = NULL;
static gboolean display_image = FALSE;  // Flag to control image display

static void load_and_set_image();
static gboolean on_draw (GtkWidget *widget, cairo_t *cr, gpointer data);
static void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static void finish_selector_stage();
static void save_coordinates(double x, double y);

static void destroy_widget(GtkWidget* widget, gpointer data){
	gtk_widget_destroy(widget);
}


/* Draws a selector box onto the drawing area (using Cairo API) */
static void draw_selector(cairo_t *cr, int x, int y) {
    cairo_set_source_rgba(cr, 0.0, 0.8, 0.0, 0.5); // Green 50% transparency
    cairo_rectangle(cr, x, y, GRID_SIZE, GRID_SIZE); // Create rectangle at position (x,y) w/ GRID_SIZE dimensions 
    cairo_fill(cr); // Fill rectangle with green
}

/* Draws the loaded image onto the drawing area (using Cairo API) */
static void draw_images(cairo_t *cr) {
	printf("Drawing images\n");
    if (image_pixbuf != NULL) {
        gdk_cairo_set_source_pixbuf(cr, image_pixbuf, image_x, image_y);
        cairo_paint(cr);
    }
}

/* Moves the selector box in intervals within window boundaries */
static void move_box(double dx, double dy) {
	printf("moving box\n");
    // Update box coordinates
    box_x += dx * GRID_SIZE;
    box_y += dy * GRID_SIZE;

    // Snap to grid
    box_x = SNAP_INTERVAL * GRID_SIZE * (int)(box_x / (SNAP_INTERVAL * GRID_SIZE));
    box_y = SNAP_INTERVAL * GRID_SIZE * (int)(box_y / (SNAP_INTERVAL * GRID_SIZE));

    // Boundary checks to ensure box stays within window space
    if (box_x < 0)
        box_x = 0;

    if (box_y < 0)
        box_y = 0;

    if (box_x + GRID_SIZE > WINDOW_WIDTH)
        box_x = WINDOW_WIDTH - GRID_SIZE;

    if (box_y + GRID_SIZE > WINDOW_HEIGHT)
        box_y = WINDOW_HEIGHT - GRID_SIZE;

    gtk_widget_queue_draw(selector_area); // Redraw the drawing area

    // Update the image display area with the loaded image
    /* if (image_pixbuf != NULL) { */
    /*     gtk_image_set_from_pixbuf(GTK_IMAGE(image_display_area), image_pixbuf); */
    /* } */
}

/* Callback function for the "draw" signal */
static gdouble last_joy_time = 0;
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("selector.png", NULL);
    if (pixbuf != NULL) {
        // Paint the image onto the drawing area
        gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
        cairo_paint(cr);

        // Release resources
        g_object_unref(pixbuf);
    } else {
        // Handle loading error
        printf("Error loading image.\n");
    }

	if(last_joy_time == 0 || (unsigned int)(g_get_monotonic_time() - last_joy_time) > 1000000/JOY_SPEED){
		printf("joy x, y: %d %d\n", joy_x, joy_y);
		move_box(joy_x*SNAP_INTERVAL, joy_y*SNAP_INTERVAL);
		last_joy_time = g_get_monotonic_time();
	}
	// Check if we need to finish selector stage
	if(btn_available == 1 && event.type == JS_EVENT_BUTTON && event.number == BTN_RIGHT_Y && event.value == 1){
		// Finish up
		btn_available = 0;
		if(last_joy_time != 0){
			if(display_image){
				save_coordinates(box_x, box_y);
				finish_selector_stage();
			}
			image_x = box_x;
            image_y = box_y;

            display_image = !display_image;  // Toggle the display_image flag

            gtk_widget_queue_draw(selector_area);

			return FALSE;
		}
	}
	// Draw selector box
    draw_selector(cr, box_x, box_y); // draw selector box

    // Draw loaded images only if the display_image flag is true
    if (display_image) {
        draw_images(cr);
    }

    return FALSE; // Indicate draw operation is complete
}

/* !!! fix to send out to gcode CAM instead of printing */
static void send_coordinates() {
    g_print("Box Coordinates: (%.0f, %.0f)\n", box_x, box_y);
}


static GList* point_path = NULL;
static void save_coordinates(double x, double y) {
	g_list_free_full(g_steal_pointer(&point_path), free);
	g_print("Box Coordinates: (%.0f, %.0f)\n", x, y);

	DoublePoint* p = malloc(sizeof(DoublePoint));

	p->x = x;
	p->y = y;

	point_path = g_list_append(point_path, p);
	points_list = g_list_prepend(points_list, point_path);
}

static void finish_selector_stage() {
	// Disconnect signal handler for keypress
	g_signal_handlers_disconnect_by_func(window, G_CALLBACK(on_key_press), NULL);
	g_signal_handlers_disconnect_by_func(vbox, G_CALLBACK(on_draw), NULL);

	// Save selector_area as a png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WINDOW_WIDTH, WINDOW_HEIGHT);
	cairo_t *cr = cairo_create(surface);
	gdk_cairo_set_source_window(cr, gtk_widget_get_window(selector_area), 0, 0);
	cairo_paint(cr);
	cairo_surface_write_to_png(surface, "selector.png");

	// Hide widgets
	gtk_widget_hide(image_display_area);
	gtk_widget_hide(selector_area);

	image_x = 0;
	image_y = 0;
	box_x = 0;
	box_y = 0;
	display_image = FALSE;

	// Change window title
	gtk_window_set_title(GTK_WINDOW(window), TITLE_GCODER);
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
            if (display_image) {
				// Finish up
				save_coordinates(box_x, box_y);
				finish_selector_stage();
				return;
            }

            image_x = box_x;
            image_y = box_y;

            display_image = !display_image;  // Toggle the display_image flag

            gtk_widget_queue_draw(selector_area);
            break;
        default:
            break;
    }
}

static void load_and_set_image() {
    if (image_file_path != NULL) {
        if (image_pixbuf != NULL) {
            g_object_unref(image_pixbuf);
            image_pixbuf = NULL;
        }
        if (preview_pixbuf != NULL) {
            g_object_unref(preview_pixbuf);
            preview_pixbuf = NULL;
        }
        GError *error = NULL;
        image_pixbuf = gdk_pixbuf_new_from_file(image_file_path, &error);
		//preview_pixbuf = gdk_pixbuf_new_from_file(image_file_path, &error); 
        if (error != NULL) {
            g_printerr("Error loading %s: %s\n", image_file_path, error->message);
            g_error_free(error);
        }

        // Set the image in the image_display_area
        //gtk_image_set_from_pixbuf(GTK_IMAGE(image_display_area), preview_pixbuf);

		// Set shrunken image in the image_pixbuf buffer
		gtk_image_set_from_pixbuf(GTK_IMAGE(image_display_area), image_pixbuf);
		image_pixbuf = gdk_pixbuf_scale_simple(
				image_pixbuf,
				GRID_SIZE, GRID_SIZE,
				GDK_INTERP_BILINEAR
				);
    }
}

void setup_selector(){
    // Create drawing area
    selector_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(selector_area, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), selector_area, TRUE, TRUE, 0);

    // Create image display area
    image_display_area = gtk_image_new();
    gtk_widget_set_size_request(image_display_area, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), image_display_area, TRUE, TRUE, 0);

	// Hide it - show later.
	gtk_widget_hide(selector_area);
	gtk_widget_hide(image_display_area);
}

void activate_selector(  GtkApplication* self,
		GParamSpec* pspec,
  gpointer user_data){

    // Load and set the image
    load_and_set_image();

    //gtk_widget_show_all(window);
	gtk_widget_show(selector_area);
	gtk_widget_show(image_display_area);
	printf("Selector should be active now.\n");

	// Connect signals to callback functions
	g_signal_connect(G_OBJECT(vbox), "draw", G_CALLBACK(on_draw), NULL);
	g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
}
