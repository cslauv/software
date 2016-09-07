'''
Created on Sep 7, 2016

This example program is used to detect shapes from a video feed

@author: Hong Luu
'''

import numpy as np
import cv2

debug_mode = True  # if debug_mode is on, print debug message
camera_value = 0    # select camera here
camera = cv2.VideoCapture(camera_value)

while True:
    if debug_mode:
        print "still going"
    
    # get feed
    frame = camera.read()[1]
    
    # get dimensions of original feed
    img_height, img_width, img_channel = frame.shape
    
    # gray filter for testing
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
    # create two copies of the original image -- one for
    # the overlay and one for the final underwater_img image
    overlay = frame.copy()

    ret,thresh = cv2.threshold(gray,127,255,0)
    
    contours,h = cv2.findContours(thresh,1,2)
    
    for cnt in contours:
        approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
        if debug_mode:
            print len(approx)
        if len(approx)==3:
            if debug_mode:
                print "triangle"
            cv2.drawContours(overlay,[cnt],0,(0,0,255),-1)      # red for triangle
        elif len(approx)==4:
            if debug_mode:
                print "square"
            cv2.drawContours(overlay,[cnt],0,(0,165,255),-1)    # orange for square
        elif len(approx)==5:
            if debug_mode:
                print "pentagon"
            cv2.drawContours(overlay,[cnt],0,(0,255,255),-1)    # yellow for pentagon
        elif len(approx)==6:
            if debug_mode:
                print "hexagon"
            cv2.drawContours(overlay,[cnt],0,(47,255,173),-1)   # light green for hexa
        elif len(approx)==7:
            if debug_mode:
                print "heptagon"
            cv2.drawContours(overlay,[cnt],0,(0,153,153),-1)    # dark green for hepta
        elif len(approx)==8:
            if debug_mode:
                print "octagon"
            cv2.drawContours(overlay,[cnt],0,(226,43,138),-1)      # purple for octogon
        elif len(approx) == 9:
            if debug_mode:
                print "nonagon"
            cv2.drawContours(overlay,[cnt],0,(255,255,0),-1)    # cyan for 9 sided
        elif len(approx) == 10:
            if debug_mode:
                print "nonagon"
            cv2.drawContours(overlay,[cnt],0,(255,0,0),-1)    # blue for star
        elif len(approx) > 16:
            if debug_mode:
                print "circle"
            cv2.drawContours(overlay,[cnt],0,(128,128,128),-1)  # gray for circle
    
    # display result
    cv2.imshow('original', frame)   # original
    cv2.imshow('gray', gray)
    cv2.imshow('result', overlay)
    
    # needed to run loop, listens for keyboard events
    k = cv2.waitKey(30)
    
    # if ESC or spacebar is pressed, exit
    if k == 27 or k == 32:
        break

############# END of While:True loop ######################    

cv2.destroyAllWindows()
camera.release()
print "done"
