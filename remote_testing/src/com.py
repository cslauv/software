'''
   CSLA ROBOSUB
   Script for communication between raspberry pi 
   and arduino
   Collaborators: Carlos Galdamez, Shian Hung
'''
import serial

ser = serial.Serial('/dev/ttyACM0',9600)

try:
	while True:
		# Get input from user on terminal
		b = raw_input(">> ")
		ser.write(b)

# Keyboard interrupt is CTRL + C
except KeyboardInterrupt:
	pass
