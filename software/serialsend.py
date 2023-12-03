import sys
import serial
import time

def parse_args():
    if len(sys.argv) < 2:
        return -1
    #print(sys.argv[1:]);
    send_to_arduino(sys.argv[1:])
    #returned = send_to_arduino(sys.argv[1:])
    #print(f"returned: {returned}")

def send_to_arduino(gcode_command):
	#print("IN python: send_to_arduino");
	ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)
	data = (f"{gcode_command}").encode()
	print(data);
	retval = ser.write(data)
	ser.close()
    # try:
        # # Open serial connection
        # ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)  # Adjust port and baudrate as needed

        # # Send G-code line with a newline character
        # newCmd = (" ".join(gcode_command)+'\0').encode();
        # print(newCmd)
        # #ser.write(bytes(newCmd, 'utf-8'))
        # ser.write(newCmd)

        # # Wait for the Arduino to process the command (adjust as needed)
        # time.sleep(.5)
        # returned = ser.readline()
        # print(returned.decode().strip())
        # ser.close()
        # if returned == "OK\n":
            # return 0
        # else:
            # return returned.decode()

        # # Close the serial port
    # except Exception as e:
        # print(f"Error: {e}")
        # return e

parse_args();
