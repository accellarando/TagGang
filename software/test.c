#include <gtk/gtk.h>

GtkWidget *area;

gboolean on_area_expose(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(widget));
    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_rectangle(cr, 0, 0, allocation.width / 10.0, allocation.height / 10.0);
    cairo_fill(cr);
    cairo_destroy(cr);

    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK DrawingArea Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), area);

    g_signal_connect(area, "expose-event", G_CALLBACK(on_area_expose), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
