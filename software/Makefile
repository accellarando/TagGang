CC = gcc
CFLAGS = -Og -g `pkg-config --cflags gtk+-x11-3.0 skeltrack-0.1 gfreenect-0.1 gudev-1.0`
LIBS = `pkg-config --libs gtk+-x11-3.0 skeltrack-0.1 gfreenect-0.1 gudev-1.0 libudev` -lm
INCLUDES = include/
SRC_SELECTOR = selector_v1.c
SRC_CANVAS = drawable_canvas.c
SRC_MAIN = main.c
SRC_GCODE = gcode.c
EXECUTABLE_SELECTOR = selector
EXECUTABLE_CANVAS = canvas
EXECUTABLE_MAIN = TagGang

all: $(EXECUTABLE_MAIN)

$(EXECUTABLE_MAIN): $(SRC_CANVAS) $(SRC_SELECTOR) $(SRC_MAIN) $(SRC_GCODE)
	$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDES) $(LIBS)

$(EXECUTABLE_SELECTOR): $(SRC_SELECTOR)
	$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDES) $(LIBS)

.PHONY: run
run: $(EXECUTABLE_SELECTOR)
	./$(EXECUTABLE_SELECTOR) images/flower.png

.PHONY: clean
clean:
	rm -f $(EXECUTABLE_MAIN) $(EXECUTABLE_SELECTOR)
