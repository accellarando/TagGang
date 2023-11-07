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