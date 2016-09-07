'''
Created on Sep 2, 2016

This example program is used to detect shapes from an image

@author: Hong Luu
'''

import numpy as np
import cv2

# img = cv2.imread('../resources/black-and-white-shapes.png')
# gray = cv2.imread('../resources/black-and-white-shapes.png',0)

img = cv2.imread('../resources/roboSUB_office.jpg')
gray = cv2.imread('../resources/roboSUB_office.jpg',0)

ret,thresh = cv2.threshold(gray,127,255,0)

contours,h = cv2.findContours(thresh,1,2)

for cnt in contours:
    approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
    print len(approx)
    if len(approx)==3:
        print "triangle"
        cv2.drawContours(img,[cnt],0,(0,0,255),-1)      # red for triangle
    elif len(approx)==4:
        print "square"
        cv2.drawContours(img,[cnt],0,(0,165,255),-1)    # orange for square
    elif len(approx)==5:
        print "pentagon"
        cv2.drawContours(img,[cnt],0,(0,255,255),-1)    # yellow for pentagon
    elif len(approx)==6:
        print "hexagon"
        cv2.drawContours(img,[cnt],0,(47,255,173),-1)   # light green for hexa
    elif len(approx)==7:
        print "heptagon"
        cv2.drawContours(img,[cnt],0,(0,153,153),-1)    # dark green for hepta
    elif len(approx)==8:
        print "octagon"
        cv2.drawContours(img,[cnt],0,(226,43,138),-1)      # purple for octogon
    elif len(approx) == 9:
        print "nonagon"
        cv2.drawContours(img,[cnt],0,(255,255,0),-1)
    elif len(approx) == 10:
        print "nonagon"
        cv2.drawContours(img,[cnt],0,(255,0,0),-1)    # blue for star
    else:
        print "circle"
        cv2.drawContours(img,[cnt],0,(128,128,128),-1)  # white for circle

cv2.imshow('gray',gray)
cv2.imshow('shapes',img)
cv2.waitKey(0)
cv2.destroyAllWindows()