# GUI PLANS
### Behind the scenes (separation of concerns for users)
- (Elsewhere?) User confirms finished 2-D drawing
- 2-D Drawing converted into image format
- (Vvectoriation) Use [Processing](https://processing.org/) to create SVG file of image
### User interactions  
- Allows users to move a grid box within a view using arrow keys and select an area with the Enter key,
with the selected area's coordinates translated to real-life canvas coordinates
- On enter: (outputs) SVG file and coordinate numbers
### Outside of GUI
- SVG files goes into CAM software to create gcode 
- Use GRBL to translate gcode instructions into motor movements

# FIX
- find out real canvas size and coordinates (fix polar/cartesian)
- find out if there needs to be any image processing
- what CAM software?


# NOTES
Reference GTK [Getting Started](https://docs.gtk.org/gtk3/getting_started.html)
- GTK window with a fixed-size selector that can be moved using arrow keys and snaps to a grid
- Arrow keys, [Widget::key-press-event](https://docs.gtk.org/gtk3/signal.Widget.key-press-event.html)

## TODO
- Open application window with commandline args takes in an image file (.png/.jpg): GApplication entry points GIO documentation... open() = virtual function, which gets called when the app is launched with commandline args.
- Mak XML backing file?
- Replaces the selector button and make a function that is called when the drawing area needs to be redrawn, and it uses Cairo to draw a highlighted rectangle based on the selector's position. The selectorHighlighted variable is used to control whether the rectangle should be drawn. The key press event now updates the selector position and triggers a redraw of the fixed container.
- images transparent 400x400


