#include <gtk/gtk.h>

// Constants for the view
#define VIEW_WIDTH  400
#define VIEW_HEIGHT 400

GtkWidget *drawing_area;
int box_x = 50;
int box_y = 50;
int box_width = 50;
int box_height = 50;
gboolean box_selected = FALSE;

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Draw the view
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White background
    cairo_paint(cr);

    // Draw the grid box
    if (box_selected) {
        cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);  // Blue color for selected box
    } else {
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  // Red color for unselected box
    }

    cairo_rectangle(cr, box_x, box_y, box_width, box_height);
    cairo_fill(cr);

    return FALSE;
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->type == GDK_KEY_PRESS) {
        switch (event->keyval) {
            case GDK_KEY_Left:
                box_x -= 10;
                break;
            case GDK_KEY_Right:
                box_x += 10;
                break;
            case GDK_KEY_Up:
                box_y -= 10;
                break;
            case GDK_KEY_Down:
                box_y += 10;
                break;
            case GDK_KEY_Return:
                // Area selection
                g_print("Selected area: x=%d, y=%d, width=%d, height=%d\n", box_x, box_y, box_width, box_height);
                box_selected = TRUE;
                break;
            default:
                break;
        }

        gtk_widget_queue_draw(widget); // Redraw the view
    }

    return TRUE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, VIEW_WIDTH, VIEW_HEIGHT);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "key-press-event", G_CALLBACK(on_key_press), NULL);
    gtk_widget_set_can_focus(drawing_area, TRUE); // Allow key events on the drawing area

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
