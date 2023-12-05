"""
serialsend:

Python script for sending gcode commands to Arduino over serial connection.
Utilizes pyserial library for easier integration (see serialsend.c for C version).

@author Dana Escandor
"""
import sys
import serial
import time

"""
parses command-line argument sent from send_commands.c to call send_to_arduino
with provided gcode command
"""
def parse_args():
    if len(sys.argv) < 2:
        return -1
    #print(sys.argv[1:]);
    send_to_arduino(sys.argv[1:])
    #returned = send_to_arduino(sys.argv[1:])
    #print(f"returned: {returned}")

"""
parses command-line argument sent from send_commands.c to be args for send_to_arduino
"""
def send_to_arduino(gcode_command):
    ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=10) # set up serial connection

    response = ser.readline().decode().strip()
    #while(len(response) != 0):
        #print(f"Arduino Response: {response}")
        #response = ser.readline().decode().strip()
    gcode_command = " ".join(gcode_command)
    data = (f"{gcode_command} \n").encode() # encode bytes for serial to read properly
    print(data);
    retval = ser.write(data) # send data to via serial Arduino
    response = ser.readline().decode().strip()
    while(len(response) != 0):
        print(f"Arduino Response: {response}")
        response = ser.readline().decode().strip()
    ser.close()

    # (debugging) printing data:
    # b"['G1 L217.503103 R1282.552845\\n']" >> before removing newline in parse_gcode
    # b"['G1 L217.503103 R1282.552845']" >> before stripping
    # b'G1 L217.503103 R1282.552845\n' >> CORRECT

# initiate script with parse_args
parse_args();
