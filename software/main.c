/**
 * This is the TagGang main program entry.
 * It moves through three different screens:
 * 1. Drawing screen
 * 2. Tag placement screen
 * 3. Confirmation and progress, with gcode generation
 * 4. Loop back to 1, retaining state as applicable
 *
 * Kinect code in this program is based on the Skeltrack example.
 * https://github.com/joaquimrocha/Skeltrack/blob/master/examples/test-kinect.c
 */
#include <main.h>

// Global/shared variables
// GtkApplication *app;
GtkWidget *window;
GFreenectDevice *kinect = NULL; 
SkeltrackSkeleton *skeleton = NULL;

void scale_point_cloud(GtkApplication *app,
		gpointer data){
	GList* paths = (GList*) data;
}

void activate_plotter(GtkApplication *app, void* hi, gpointer data){
}


/**
 * This function routes Title Change signals to the appropriate
 * function, based on the current state of the program.
 */
void signal_router(GtkApplication *app,
		gpointer data){
	if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_SELECTOR) == 0){
		activate_selector(app, NULL, data);
	}
	else if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_GCODER) == 0){
		activate_gcoder(app, NULL, data);
	}
	else if(strcmp(gtk_window_get_title(GTK_WINDOW(window)), TITLE_PLOTTER) == 0){
		activate_plotter(app, NULL, data);
	}
	else {
		activate_canvas(app, NULL, data);
	}
}

void activate(GtkApplication *app,
		gpointer data){
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Draw a picture!");
	gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Route signals that the child functions emit by changing window title
	g_signal_connect (window, "notify::title", G_CALLBACK(signal_router), app);

	// Start with the canvas
	activate_canvas(app, NULL, window);
}

static BufferInfo *process_buffer (guint16 *buffer,
		guint width,
		guint height,
		guint dimension_factor,
		guint threshold_begin,
		guint threshold_end)
{
	BufferInfo *buffer_info;
	gint i, j, reduced_width, reduced_height;
	guint16 *reduced_buffer;

	g_return_val_if_fail (buffer != NULL, NULL);

	reduced_width = (width - width % dimension_factor) / dimension_factor;
	reduced_height = (height - height % dimension_factor) / dimension_factor;

	reduced_buffer = g_slice_alloc0 (reduced_width * reduced_height *
			sizeof (guint16));

	for (i = 0; i < reduced_width; i++)
	{
		for (j = 0; j < reduced_height; j++)
		{
			gint index;
			guint16 value;

			index = j * width * dimension_factor + i * dimension_factor;
			value = buffer[index];

			if (value < threshold_begin || value > threshold_end)
			{
				reduced_buffer[j * reduced_width + i] = 0;
				continue;
			}

			reduced_buffer[j * reduced_width + i] = value;
		}
	}

	buffer_info = g_slice_new0 (BufferInfo);
	buffer_info->reduced_buffer = reduced_buffer;
	buffer_info->reduced_width = reduced_width;
	buffer_info->reduced_height = reduced_height;
	buffer_info->width = width;
	buffer_info->height = height;

	return buffer_info;
}
static void on_track_joints (GObject      *obj,
		GAsyncResult *res,
		gpointer      user_data)
{
	guint i;
	BufferInfo *buffer_info;
	guint16 *reduced;
	gint width, height, reduced_width, reduced_height;
	GError *error = NULL;

	buffer_info = (BufferInfo *) user_data;
	reduced = (guint16 *) buffer_info->reduced_buffer;
	width = buffer_info->width;
	height = buffer_info->height;
	reduced_width = buffer_info->reduced_width;
	reduced_height = buffer_info->reduced_height;

	list = skeltrack_skeleton_track_joints_finish (skeleton,
			res,
			&error);

	if (error == NULL)
	{
		// Do something in here i think
	}
	else
	{
		g_warning ("%s\n", error->message);
		g_error_free (error);
	}

	g_slice_free1 (reduced_width * reduced_height * sizeof (guint16), reduced);

	g_slice_free (BufferInfo, buffer_info);

	skeltrack_joint_list_free (list);
}

static void on_depth_frame (GFreenectDevice *dev, gpointer data){
	gboolean smoothing_enabled;
	gint width, height;
	gint dimension_factor;
	guchar *grayscale_buffer;
	guint16 *depth;
	BufferInfo *buffer_info;
	gsize len;
	GError *error = NULL;
	GFreenectFrameMode frame_mode;
	ClutterContent *content;

	depth = (guint16 *) gfreenect_device_get_depth_frame_raw (dev,
			&len,
			&frame_mode);

	width = frame_mode.width;
	height = frame_mode.height;

	g_object_get (skeleton, "dimension-reduction", &dimension_factor, NULL);

	buffer_info = process_buffer (depth,
			width,
			height,
			dimension_factor,
			THRESHOLD_BEGIN,
			THRESHOLD_END);

	skeltrack_skeleton_track_joints (skeleton,
			buffer_info->reduced_buffer,
			buffer_info->reduced_width,
			buffer_info->reduced_height,
			NULL,
			on_track_joints,
			buffer_info);
}

static void on_video_frame (GFreenectDevice *kinect, gpointer user_data)
{
	// we don't really care about video frames
	// do nothing for now
}

static void on_new_kinect(GObject *obj,
		GAsyncResult *res,
		gpointer data){

	// Instantiate kinect
	GError *err = NULL;
	kinect = gfreenect_device_new_finish(res, &err);
	if(err){
		g_print("Failed to instantiate kinect device: %s\n", err->message);
		g_error_free(err);
		return;
	}

	// Instantiate skeleton
	skeleton = skeltrack_skeleton_new();
	g_object_get(skeleton, "smoothing-factor", &smoothing_factor, NULL);

	// Set up signal handlers from kinect
	g_signal_connect (kinect,
			"depth-frame",
			G_CALLBACK (on_depth_frame),
			NULL);

	g_signal_connect (kinect,
			"video-frame",
			G_CALLBACK (on_video_frame),
			NULL);

	gfreenect_device_set_tilt_angle(kinect, 0, NULL, NULL, NULL);
	gfreenect_device_start_depth_stream(kinect, 
			GFREENECT_DEPTH_FORMAT_MM, 
			NULL);
	gfreenect_device_start_video_stream(kinect, 
			GFREENECT_RESOLUTION_MEDIUM,
			GFREENECT_VIDEO_FORMAT_RGB,
			NULL);
}

int main (int    argc,
		char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.taggang.main", G_APPLICATION_FLAGS_NONE);

	// Setup Kinect device
	gfreenect_device_new(0,
			GFREENECT_SUBDEVICE_CAMERA,
			NULL,
			on_new_kinect,
			NULL);

	// Set up activation signal handler
	g_signal_connect (app, "activate", G_CALLBACK (activate), window);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
