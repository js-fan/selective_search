from selective_search import selective_search
import matplotlib.pyplot as plt
import numpy as np
import cv2
import time

tic = time.time()
boxes = selective_search('./demo/skimage_astronaut.jpg', 0, 20)
toc = time.time()
print boxes.shape, toc - tic


image = cv2.imread('./demo/skimage_astronaut.jpg')
boxes2 = selective_search(image, 0, 20)
toc = time.time()
print boxes2.shape, toc - tic


