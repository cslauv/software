'''
Created on Aug 31, 2016

@author: Hong Luu
'''

import cv2
import numpy as np
import matplotlib.pyplot as matplot

video = cv2.VideoCapture(1)
underwater_color = (255, 0, 0)

while True:
    print "still going"
    
    # get feed
    frame = video.read()[1]
    
    # get dimensions of original feed
    img_height, img_width, img_channel = frame.shape

    # gray filter for testing
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # create two copies of the original image -- one for
    # the overlay and one for the final underwater_img image
    overlay = frame.copy()
    underwater_img = frame.copy()

    # draw a rectangle in the image, BGR
    cv2.rectangle(overlay, (0, 0), (img_width, img_height),
        underwater_color, -1)
    
    # apply the overlay
    cv2.addWeighted(overlay, 0.3, underwater_img, 0.8,
        0, underwater_img)

    # display result
    cv2.imshow('original', frame)   # original
    cv2.imshow('gray', gray)
    cv2.imshow('underwater', underwater_img)
    
    # needed to run loop, listens for keyboard events
    k = cv2.waitKey(30)
    
    # if ESC or spacebar is pressed, exit
    if k == 27 or k == 32:
        break
    
    
# close windows
cv2.destroyAllWindows()
video.release()

print "done"