# main.c script updates the int variable, d_pos (private to the move_stepper function), in real time:
# using Python's subprocess module to run the C script as a subprocess and capture its output.
# This output is the step value needed to step the motor in Arduino, which is done through
# serial connections between the Python and Arudino scripts.

#!/usr/bin/python3
# Usage: ./serialMotorCMD.py numberOfSteps
import sys
import serial
import time

def parse_args():
    if len(sys.argv) < 2:
        return -1
    returned = send_to_arduino(sys.argv[1:])
    print(f"returned: {returned}")

def send_to_arduino(gcode_command):
    try:
        # Open serial connection
        ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)  # Adjust port and baudrate as needed

        # Send G-code line with a newline character
        newCmd = (" ".join(gcode_command)+'\0').encode();
        print(newCmd)
        #ser.write(bytes(newCmd, 'utf-8'))
        ser.write(newCmd)

        # Wait for the Arduino to process the command (adjust as needed)
        time.sleep(.5)
        returned = ser.readline()
        print(returned.decode().strip())
        ser.close()
        if returned == "OK\n":
            return 0
        else:
            return returned.decode()

        # Close the serial port
    except Exception as e:
        print(f"Error: {e}")
        return e

parse_args();
