#include <gtk/gtk.h>
#include <math.h>

#define GRID_SIZE 50
#define SNAP_INTERVAL 0.5
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

/* Global variables */
static GtkWidget *drawing_area;
static double box_x = 0; // x-coordinates of the box
static double box_y = 0; // y-coordinates of the box

/* */
static void draw_highlight_box(cairo_t *cr, int x, int y) {
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
    
    // snap?
    box_x = SNAP_INTERVAL* GRID_SIZE * (int)(box_x / (SNAP_INTERVAL * GRID_SIZE));
    box_y = SNAP_INTERVAL* GRID_SIZE * (int)(box_y / (SNAP_INTERVAL * GRID_SIZE));
    
    if (box_x < 0)
		box_x = 0;
		
    if (box_y < 0)
		box_y = 0;
    
    if (box_x + GRID_SIZE > WINDOW_WIDTH)
		box_x = WINDOW_WIDTH - GRID_SIZE;
		
    if (box_y + GRID_SIZE > WINDOW_HEIGHT)
		box_y = WINDOW_HEIGHT - GRID_SIZE;


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
