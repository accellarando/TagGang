CC = gcc
CFLAGS = `pkg-config --cflags gtk+-x11-3.0`
LIBS = `pkg-config --libs gtk+-x11-3.0`
SRC = selector_v1.c
EXECUTABLE = selector

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE) stick-figure.png

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
