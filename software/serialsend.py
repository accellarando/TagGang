"""
serialsend:

Python script for sending gcode commands to Arduino over serial connection.
Utilizes pyserial library for easier integration (see serialsend.c for C version).

@author Dana Escandor
"""
import sys
import serial
import time
import termios

"""
parses command-line argument sent from send_commands.c to call send_to_arduino
with provided gcode command
"""
def parse_args():
    if len(sys.argv) < 2:
        return -1
    #print(sys.argv[1:]);
    return send_to_arduino(sys.argv[1:])
    #returned = send_to_arduino(sys.argv[1:])
    #print(f"returned: {returned}")

"""
parses command-line argument sent from send_commands.c to be args for send_to_arduino

see https://stackoverflow.com/questions/15460865/disable-dtr-in-pyserial-from-code
(we have to disable the DTR pulse or the Arduino resets)
"""
def send_to_arduino(gcode_command):
    port = '/dev/ttyACM0'
    f = open(port)
    attrs = termios.tcgetattr(f)
    attrs[2] = attrs[2] & ~termios.HUPCL
    termios.tcsetattr(f, termios.TCSAFLUSH, attrs)
    f.close()
    ser = serial.Serial()
    ser.baudrate = 9600
    ser.port = port
    ser.timeout = 20 
    ser.open()

    gcode_command = " ".join(gcode_command)
    data = (f"{gcode_command} \n").encode() # encode bytes for serial to read properly
    print(data);
    retval = ser.write(data) # send data to via serial Arduino
    response = ser.readline().decode().strip()
    error = 0
    while(len(response) != 0):
        print(f"Arduino Response: {response}")
        if response[0] == "O":
            error = 0
            break
        elif response[0] == "E":
            error = response[1:]
            break
        response = ser.readline().decode().strip()
    ser.close()
    return error


# initiate script with parse_args
if __name__ == '__main__':
    parse_args();
