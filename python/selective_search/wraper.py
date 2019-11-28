from ctypes import cdll, c_int, c_ubyte, byref, POINTER
import numpy as np
import os
libpath = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        'selective_search.so')
_ss = cdll.LoadLibrary(libpath)
_ss.selective_search_src.restype = POINTER(c_int)
_ss.selective_search_mat.restype = POINTER(c_int)


def selective_search(image, mode, min_size=20):
    '''
    mode:   0, fast
            1, quality
    '''
    num_box = c_int(0)

    if isinstance(image, str):
        _boxes = _ss.selective_search_src(image, mode, byref(num_box), min_size)
    else:
        assert image.dtype == np.uint8 and image.shape[2] == 3
        _boxes = _ss.selective_search_mat(image.ravel().ctypes.data_as(POINTER(c_ubyte)),
                image.shape[0], image.shape[1], mode, byref(num_box), min_size)

    if num_box.value > 0:
        boxes = np.array(_boxes[:num_box.value * 4]).reshape(-1, 4).copy()
        boxes[:, 2:4] += 1
    else:
        boxes = None
    _ss.clear(_boxes)
    return boxes
