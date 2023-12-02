
# import serial
# import time

# arduino = serial.Serial(port='/dev/ttyACM0', baudrate= 9600, timeout= 1)

# def write_read(x):
#     arduino.write(bytes(x, 'utf-8'))
#     time.sleep(0.05)
#     data = arduino.readline()
#     return data


# while True:
#     num = input("Enter a num: ")
#     value = write_read(num)
#     print(value.decode().strip())
#!/usr/bin/python3
# WAS move_stepper.py 

# https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
# https://github.com/accellarando/TagGangPrototype/blob/main/shellOutput.txt\
# https://github.com/accellarando/TagGangPrototype/blob/main/main.c
# https://howtomechatronics.com/tutorials/arduino/stepper-motors-and-arduino-the-ultimate-guide/

# serial comm between py and ino
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

# C to Python
    # https://www.digitalocean.com/community/tutorials/how-to-use-subprocess-to-run-external-programs-in-python-3
    # https://stackoverflow.com/questions/42993038/using-subprocess-call-to-read-output-of-c-executable-print-statement-realtime
    

# main.c script updates the int variable, d_pos (private to the move_stepper function), in real time:
# using Python's subprocess module to run the C script as a subprocess and capture its output.
# This output is the step value needed to step the motor in Arduino, which is done through
# serial connections between the Python and Arudino scripts.

#!/usr/bin/python3
# Usage: ./serialMotorCMD.py numberOfSteps
import subprocess
import serial # PySerial library to open a serial connection
import sys
import time

def parse_args():
    if len(sys.argv) < 2:
        return -1
    send_to_arduino(sys.argv[1])

# Function: send step value from Python to Arduino via serial communication
def send_to_arduino(steps):
     # Open and configure serial connection (change as needed) using the PySerial library
     ser = serial.Serial(port = '/dev/ttyACM0', baudrate = 115200, timeout = 1) # Linux port

     # Convert the steps value to a byte string and send it to the Arduino, with a \n char
     # e.g., -1 -> b'-1'
     data = (str(steps)+'\n').encode()
     retval = ser.write(data)

     # Close the serial port
     ser.close()

# Call the parse_args() function to start the program.
parse_args();
