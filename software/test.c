#include <gtk/gtk.h>

#define GRID_SIZE 50

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *highlightBox;

int boxX = 0;
int boxY = 0;

void moveBox(int x, int y) {
    boxX = x;
    boxY = y;

    gtk_fixed_move(GTK_FIXED(fixed), highlightBox, boxX, boxY);
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KEY_Left:
            moveBox(boxX - GRID_SIZE, boxY);
            break;

        case GDK_KEY_Right:
            moveBox(boxX + GRID_SIZE, boxY);
            break;

        case GDK_KEY_Up:
            moveBox(boxX, boxY - GRID_SIZE);
            break;

        case GDK_KEY_Down:
            moveBox(boxX, boxY + GRID_SIZE);
            break;

        default:
            break;
    }

    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Grid Snapping");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a fixed container to hold other widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    // Create the green highlight box
    highlightBox = gtk_event_box_new();
    GdkColor color;
    gdk_color_parse("green", &color);
    gtk_widget_modify_bg(highlightBox, GTK_STATE_NORMAL, &color);
    gtk_widget_set_size_request(highlightBox, GRID_SIZE, GRID_SIZE);
    gtk_fixed_put(GTK_FIXED(fixed), highlightBox, boxX, boxY);

    // Connect the key press event
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
