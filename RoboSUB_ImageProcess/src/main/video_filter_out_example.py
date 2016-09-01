'''
Created on Aug 31, 2016

@author: Hong Luu
'''

import cv2
import numpy as np
import matplotlib.pyplot as matplot

video = cv2.VideoCapture(1)

while True:
    _, frame = video.read()
    
    # hsv = hue saturation value
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([255,255,255])     # tweak numbers
    upper_red = np.array([255,255,255])     # tweak numbers
    
    mask = cv2.inRange(hsv, lower_red, upper_red)
    res = cv2.bitwise_and(frame, frame, mask = mask)
    
    cv2.imshow('original', frame)   # original
    cv2.imshow('mask', mask)        # hsv filter
    cv2.imshow('res', res)          # true/false representation of filter
    
    # press a button to exit
    if cv2.waitKey(0):
        break
    
# close windows
cv2.destroyAllWindows()
video.release()