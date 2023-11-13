// Template main.c, example-3.c (w/out XML)

#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

// Global vars
GtkWidget *selector;
int gridSize = 32; // Adjust accordingly
int selectorSize = 32; // Adjust accordingly
int maxX, maxY;

gboolean
key_press_event (GtkWidget* self,
                GdkEventKey event,
                gpointer user_data)
{
    int x, y;
    gtk_widget_get_position(selector, &x, &y);

    switch(event->keyval) // From struct GdkEventKey
    {
        case GDK_KEY_Up:
            y -= gridSize;
            break;
        case GDK_KEY_Down:
            y += gridSize;
            break;
        case GDK_KEY_Left:
            x -= gridSize;
            break;
        case GDK_KEY_Right:
            x += gridSize;
            break;
        default:
            return FALSE;  // Ignore other keys for now
    }
    
    // Clamps the selector within the boundary
    x = MAX(0, MIN(x, maxX - selectorSize));
    y = MAX(0, MIN(y, maxY - selectorSize));

    // Snap to the grid
    x = (x / gridSize) * gridSize;
    y = (y / gridSize) * gridSize;

    // Set new position
    gtk_fixed_move(GTK_FIXED(widget), selector, x, y);

    return TRUE;
}


int
main (int   argc,
      char *argv[])
{
  //GtkBuilder *builder;
  GObject *window;
  GtkWidget *fixed;
  //GObject *button;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Grid Selector");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  fixed = gtk_fixed_new();



  /* Construct a GtkBuilder instance and load our UI description ... TODO
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    } */

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  gtk_main ();

  return 0;
}
