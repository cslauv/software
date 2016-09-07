'''
Created on Aug 31, 2016

@author: Hong Luu
'''

import cv2
import numpy as np
import matplotlib.pyplot as matplot

img = cv2.imread("../resources/watch.jpg")
gray = cv2.imread("../resources/watch.jpg", cv2.IMREAD_GRAYSCALE)
green = cv2.imread("../resources/watch.jpg", cv2.IMREAD_COLOR)

img_height, img_width, img_channel = img.shape

# # loop over the alpha transparency values
# for alpha in np.arange(0, 1.1, 0.1)[::-1]:
#     # create two copies of the original image -- one for
#     # the overlay and one for the final underwater_img image
#     overlay = img.copy()
#     underwater_img = img.copy()
#  
#     # draw a rectangle in the image
#     cv2.rectangle(overlay, (0, 0), (img_width, img_height),
#         (0, 0, 255), -1)
#     # apply the overlay
#     cv2.addWeighted(overlay, alpha, underwater_img, 1 - alpha,
#         0, underwater_img)
#     # show the underwater_img image
#     print("alpha={}, beta={}".format(alpha, 1 - alpha))
#     cv2.imshow("Output", underwater_img)
#     cv2.waitKey(0)

# create two copies of the original image -- one for
# the overlay and one for the final underwater_img image
overlay = img.copy()
underwater_img = img.copy()

# draw a rectangle in the image, BGR
cv2.rectangle(overlay, (0, 0), (img_width, img_height),
    (200, 200, 16), -1)

# apply the overlay
cv2.addWeighted(overlay, 0.3, underwater_img, 0.8,
    0, underwater_img)


cv2.imshow('original', img)
cv2.imshow('gray', gray)
cv2.imshow('green', underwater_img)
cv2.waitKey(0)
cv2.destroyAllWindows()

# matplot.imshow(img, cmap='gray', interpolation='bicubic')
# matplot.imshow(img, cmap='gray', interpolation='bicubic')
# matplot.imshow(img, cmap='gray', interpolation='bicubic')
# matplot.show()
