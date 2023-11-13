#include <gtk/gtk.h>

// Global variables
GtkWidget *fixed;
GtkWidget *drawing_area;
int gridSize = 32; // Adjust accordingly
int maxX, maxY;
int selectorX = 0, selectorY = 0;
gboolean selectorHighlight = FALSE;

gboolean draw_selector(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
	if (selectorHighlight)
	{
		cairo_set_source_rgba(cr, 0.0, 0.8, 0.0, 0.5); // Green with 50% transparency
		cairo_rectangle(cr, selectorX, selectorY, gridSize, gridSize);
		cairo_fill(cr);
		selectorHighlight = FALSE;
	}
    return TRUE;
}

gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KEY_Up:
            selectorY -= gridSize;
            g_print("UP\n");
            selectorHighlight = TRUE;
            break;
        case GDK_KEY_Down:
            selectorY += gridSize;
            g_print("DOWN\n");
            selectorHighlight = TRUE;
            break;
        case GDK_KEY_Left:
            selectorX -= gridSize;
            g_print("LEFT\n");
            selectorHighlight = TRUE;
            break;
        case GDK_KEY_Right:
            selectorX += gridSize;
            g_print("RIGHT\n");
            selectorHighlight = TRUE;
            break;
        default:
            return FALSE; // Ignore other keys for now
    }

    // Clamps the selector within the boundary
    selectorX = MAX(0, MIN(selectorX, maxX - gridSize));
    selectorY = MAX(0, MIN(selectorY, maxY - gridSize));

    // Snap to the grid
    selectorX = (selectorX / gridSize) * gridSize;
    selectorY = (selectorY / gridSize) * gridSize;

    // Redraw the fixed container
    gtk_widget_queue_draw(GTK_WIDGET(fixed));

    return TRUE;
}

gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    draw_selector(widget, cr, user_data);
    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Grid Selector");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 400);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_cb), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), drawing_area, 0, 0);

    // Connect the key press event
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(key_press_event), NULL);

    // Get the size of the fixed container
    gtk_widget_get_size_request(fixed, &maxX, &maxY);

    // Show all the widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}
