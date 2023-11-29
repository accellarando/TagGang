/*
 * This is a GTK+ program that allows you to draw on a canvas
 * with the mouse. It will also support undoing the previous stroke.
 * 
 * In the future, it will also include support via Skeltrack with an Xbox Kinect.
 *
 * Code sample starting point from https://docs.gtk.org/gtk3/getting_started.html#custom-drawing
 *
 * todo:
 * Make BUTTON_ERASE work to undo last path
 *
 * @author Ella Moss
 */
#include <drawable_canvas.h>

cairo_surface_t  *surface		= NULL;
GtkApplication	 *app			= NULL;
GtkWidget *frame;
GtkWidget *drawing_area;

/***
 * This function clears the drawing off of the surface.
 */
static void clear_surface(){
	cairo_t *cr;
	cr = cairo_create(surface);

	//Sets drawing color to white
	cairo_set_source_rgb(cr, 1, 1, 1);

	cairo_paint(cr);
	cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget         *widget,
		GdkEventConfigure *event,
		gpointer           data)
{
	if (surface)
		cairo_surface_destroy (surface);

	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
			CAIRO_CONTENT_COLOR,
			gtk_widget_get_allocated_width (widget),
			gtk_widget_get_allocated_height (widget));

	/* Initialize the surface to white */
	clear_surface ();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb (GtkWidget *widget,
		cairo_t   *cr,
		gpointer   data)
{
	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);

	return FALSE;
}

/* This list contains all the points so that when they're ready to move on,
 * you can draw them all at once.
 * points_list:
 * { { {0,0}, {0,1}, ...},
 *	 { {5, 6}, {7,8} ...}}
 */
GList* points_list = NULL;
GList* current_path = NULL;
static gdouble last_x = -1.0;
static gdouble last_y = -1.0;
/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget,
		gdouble    x,
		gdouble    y)
{
	// Save this point to the path
	DoublePoint* p = malloc(sizeof(DoublePoint));
	p->x = x;
	p->y = y;
	current_path = g_list_append(current_path, p);

	cairo_t *cr;
	
	/* Paint to the surface, where we store our state */
	cr = cairo_create (surface);
	cairo_set_line_width(cr, 6);

	// If last_x and last_y are not -1, then draw from last coordinate to this one.
	if(last_x != -1 && last_y != -1){
		gdouble dx = x - last_x;
		gdouble dy = y - last_y;
		//printf("dx: %f, dy: %f, last_x: %f, last_y: %f\n", dx, dy, last_x, last_y);
		cairo_new_path(cr);
		cairo_move_to(cr, last_x, last_y);
		cairo_line_to(cr, x, y);
		cairo_stroke(cr);
		/* Now invalidate the drawing area along the line. */
		gtk_widget_queue_draw_area (widget, MIN(last_x, x) - 12, MIN(last_y, y) - 12, ABS(dx)+24.0, ABS(dy)+24.0);
	}
	else{
		cairo_rectangle (cr, x - 3, y - 3, 6, 6);
		cairo_fill (cr);
		gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);
	}
	cairo_destroy (cr);

	last_x = x;
	last_y = y;
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */

#define BUTTON_PEN_DOWN GDK_BUTTON_PRIMARY
#define BUTTON_ERASE GDK_BUTTON_SECONDARY
#define BUTTON_SAVE GDK_BUTTON_MIDDLE
static gboolean button_press_event_cb (GtkWidget      *widget,
		GdkEventButton *event,
		gpointer        data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL)
		return FALSE;

	if (event->button == BUTTON_PEN_DOWN)
	{
		draw_brush (widget, event->x, event->y);
	}
	else if (event->button == BUTTON_ERASE)
	{
		clear_surface ();
		gtk_widget_queue_draw (widget);
	}
	else if(event->button == BUTTON_SAVE){
		// Save the current drawing to a file
		cairo_surface_write_to_png(surface, "drawing.png");
		// Clear the screen
		clear_surface();
		gtk_widget_queue_draw (widget);

		// Prepare for moving to next stage
		if (surface)
			cairo_surface_destroy (surface);
		/* gtk_widget_destroy(widget); */
		gtk_widget_destroy(frame);
		gtk_widget_destroy(drawing_area);

		
		// Change window title to next stage.
		// This also triggers the signal router for you.
		gtk_window_set_title(window, TITLE_SELECTOR);
	}

	/* We've handled the event, stop processing */
	return TRUE;
}

/* Handle button release event by resetting last coordinates to
 * -1.0, -1.0 if it was button 1 (left click) that was released.
 */
static gboolean button_release_event_cb (GtkWidget      *widget,
		GdkEventButton *event,
		gpointer        data)
{
	if (event->button == BUTTON_PEN_DOWN)
	{
		//printf("Button released\n");
		// Add the current path to the list of paths
		points_list = g_list_append(points_list, current_path);
		current_path = NULL;
		last_x = -1.0;
		last_y = -1.0;
	}

	return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget      *widget,
		GdkEventMotion *event,
		gpointer        data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL)
		return FALSE;

	if (event->state & GDK_BUTTON1_MASK)
		draw_brush (widget, event->x, event->y);

	/* We've handled it, stop processing */
	return TRUE;
}

static void close_window (void)
{
	if (surface)
		cairo_surface_destroy (surface);
}

void activate_canvas (GtkApplication *app,
		void*		i_forgot,
		gpointer	 user_data)
{
	window = (GtkWidget*) user_data;

	g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

	gtk_container_set_border_width (GTK_CONTAINER (window), 8);

	frame = gtk_frame_new (NULL);
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
	gtk_container_add (GTK_CONTAINER (window), frame);

	drawing_area = gtk_drawing_area_new ();
	/* set a minimum size */
	gtk_widget_set_size_request (drawing_area, 500, 500);

	gtk_container_add (GTK_CONTAINER (frame), drawing_area);

	/* Signals used to handle the backing surface */
	g_signal_connect (drawing_area, "draw",
			G_CALLBACK (draw_cb), NULL);
	g_signal_connect (drawing_area,"configure-event",
			G_CALLBACK (configure_event_cb), NULL);

	/* Event signals */
	g_signal_connect (drawing_area, "motion-notify-event",
			G_CALLBACK (motion_notify_event_cb), NULL);
	g_signal_connect (drawing_area, "button-press-event",
			G_CALLBACK (button_press_event_cb), NULL);
	g_signal_connect (drawing_area, "button-release-event",
			G_CALLBACK (button_release_event_cb), NULL);

	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to. In particular, we need to ask for the
	 * button press and motion notify events that want to handle.
	 */
	gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
			| GDK_POINTER_MOTION_MASK);

	gtk_widget_show_all (window);
}

