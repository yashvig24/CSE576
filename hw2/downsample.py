import numpy as np
import cv2

image = cv2.imread('data/shabi1.jpeg')
#cv2.imshow("Original", image)
"""
The ratio is r. The new image will
have a height of 50 pixels. To determine the ratio of the new
height to the old height, we divide 50 by the old height.
"""
r = 50.0 / image.shape[0]
dim = (int(image.shape[1] * r), 50)

resized = cv2.resize(image, (517,388), interpolation = cv2.INTER_AREA)
cv2.imshow("Resized (Height) ", resized)
cv2.waitKey(0)
