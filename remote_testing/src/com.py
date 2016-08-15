'''
   CSLA ROBOSUB
   Script for communication between raspberry pi 
   and arduino
   Collaborators: Carlos Galdamez, Shian Hung
'''
import serial

arduino = serial.Serial('/dev/ttyACM0',9600)

try:
	while True:		
		# Get input from user on terminal
		command = raw_input(">> ")
		arduino.write(command)
                
# Keyboard interrupt is CTRL + C
except KeyboardInterrupt:
	pass
