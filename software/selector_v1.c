#include <gtk/gtk.h>

#define GRID_SIZE 50
#define SNAP_INTERVAL 0.1

static GtkWidget *drawing_area;
static double box_x = 0;
static double box_y = 0;

static void draw_highlight_box(cairo_t *cr, double x, double y) {
    cairo_set_source_rgb(cr, 0, 1, 0); // Set color to green
    cairo_rectangle(cr, x, y, GRID_SIZE, GRID_SIZE);
    cairo_fill(cr);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    draw_highlight_box(cr, box_x, box_y);
    return FALSE;
}

static void move_box(double dx, double dy) {
    box_x += dx * GRID_SIZE;
    box_y += dy * GRID_SIZE;

    // Snap to grid
    box_x = SNAP_INTERVAL * GRID_SIZE * floor(box_x / (SNAP_INTERVAL * GRID_SIZE));
    box_y = SNAP_INTERVAL * GRID_SIZE * floor(box_y / (SNAP_INTERVAL * GRID_SIZE));

    gtk_widget_queue_draw(drawing_area);
}

static void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
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
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Snapping Box");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    gtk_widget_set_events(window, GDK_KEY_PRESS_MASK);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
