'''
Created on Oct 3, 2016

This example program is used to detect color from an image
Reference:
    http://opencv-python-tutroals.readthedocs.io/
        en/latest/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html

@author: Hong Luu
'''


import cv2
import numpy as np

camera = cv2.VideoCapture(0)

# define range of red color in BGR
#lower_color_bound = np.array([17, 10, 150])
#upper_color_bound = np.array([38, 30, 215])
lower_color_bound = np.array([0, 0, 0])
upper_color_bound = np.array([255, 255, 255])

while(1):

    # Take each frame
    _, frame = camera.read()

    #frame = cv2.imread('../resources/red_img.jpg')

    # Threshold the HSV image to get only red colors
    mask = cv2.inRange(frame, lower_color_bound, upper_color_bound)

    # Bitwise-AND mask and original image
    res = cv2.bitwise_or(frame,frame, mask= mask)

    cv2.imshow('frame',frame)
    cv2.imshow('mask',mask)
    cv2.imshow('res',res)
    k = cv2.waitKey(5) & 0xFF

    # if ESC or spacebar is pressed, exit
    if k == 27 or k == 32:
        break

cv2.destroyAllWindows()
camera.release()

print "done"