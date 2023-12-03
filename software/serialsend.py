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
	ser = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=1)
	gcode_command = gcode_command[0].strip("[]'")
	data = (f"{gcode_command}\n").encode()
	#print(data);
	retval = ser.write(data)
	
	response = ser.readline().decode().strip()
	print(f"Arduino Response: {response}")
		
	ser.close()
	
	# printing data:
	# b"['G1 L217.503103 R1282.552845\\n']" >> before removing newline in parse_gcode
	# b"['G1 L217.503103 R1282.552845']" >> before stripping
	# b'G1 L217.503103 R1282.552845\n' >> CORRECT

parse_args();
