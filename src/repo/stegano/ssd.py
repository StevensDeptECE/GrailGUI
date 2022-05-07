#!/bin/python
import cv2
import numpy as np

img1 = cv2.imread('hubble.webp')
img2 = cv2.imread('newhubble.webp')
img3 = cv2.imread('coloredhubble.webp')


# Calculate the sum of squares of the differences between two images
def ssd(img1, img2):
    return np.sum(cv2.subtract(img1, img2))


print('ssd of images:', ssd(img1, img2))
print('ssd of images:', ssd(img1, img3))
