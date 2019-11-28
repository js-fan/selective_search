import sys;sys.path.append('./python')

from selective_search import selective_search
import numpy as np
import cv2
import time

'''
boxes = selective_search(image, mode, min_size)
>> INPUT
    image:  src (str) or NumpyArray (3d uint8 tensor, H x W x 3).
    mode:   0 (fast mode) or 1 (quality mode).
>> OUTPUT
    boxes:  NumpyArray (2d tensor, N x 4)
            or None if no boxes are found.
'''

# load from src is faster
tic = time.time()
boxes = selective_search('./demo/skimage_astronaut.jpg', 0, 20)
toc = time.time()
print('box.shape:', boxes.shape, 'time:', toc - tic)


image = cv2.imread('./demo/skimage_astronaut.jpg')
boxes2 = selective_search(image, 0, 20)
toc = time.time()
print('box.shape:', boxes2.shape, 'time:', toc - tic)


