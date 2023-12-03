// TESTING FILE TO BE INTEGRATED INTO send_commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define SERIAL_PORT "/dev/ttyACM0"
#define BAUDRATE 115200
#define BUFFER_SIZE 256

int open_serial_port() {
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error opening serial port");
        exit(EXIT_FAILURE);
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        exit(EXIT_FAILURE);
    }

    cfsetospeed(&tty, BAUDRATE);
    cfsetispeed(&tty, BAUDRATE);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
        exit(EXIT_FAILURE);
    }

    return fd;
}

void send_command(int serial_fd, const char *command) {
    char buffer[BUFFER_SIZE];
    int n;

    // Send command to Arduino
    char modifiedCommand[BUFFER_SIZE];
    snprintf(modifiedCommand, sizeof(modifiedCommand), "%s\n", command);
    n = write(serial_fd, modifiedCommand, strlen(modifiedCommand));
    if (n < 0) {
        perror("Error writing to serial port");
        exit(EXIT_FAILURE);
    }

    // Wait for the response
    n = read(serial_fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("Error reading from serial port");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0'; // Null-terminate the received data
    printf("%s", buffer);
}

int main() {
    // Open the serial port
    int serial_fd = open_serial_port();

    // Open the gcode file
    FILE *gcode_file = fopen("/home/ella/Documents/gtext.gcode", "r");
    if (gcode_file == NULL) {
        perror("Error opening gcode file");
        exit(EXIT_FAILURE);
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), gcode_file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline characters
        send_command(serial_fd, line);
    }

    // Close the serial port
    close(serial_fd);

    // Close the gcode file
    fclose(gcode_file);

    return 0;
}
