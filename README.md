# Selective Search
Fast C++ implementation of selective search with python wrapper.

### Dependencies
- GCC / Clang
- Opencv
- Python2 and Numpy

### Usage
Firstly, config your opencv path if necessory in the `Makefile`:
```
OPENCV_INCLUDE = -I/your/opencv/include
OPENCV_LIBRARY = -L/your/opencv/lib
```
and compile the codes:
```sh
$ cd selective_search
$ make -j8
```

Then, run the demo:
```sh
$ ./example
```
or
```sh
$ python2 example.py
```

### Demo
![demo]('./demo/box.jpg')

### References
[1] Felzenszwalb P F, Huttenlocher D P. Efficient graph-based image segmentation[J]. International journal of computer vision, 2004, 59(2): 167-181.
[2] Uijlings J R R, Van De Sande K E A, Gevers T, et al. Selective search for object recognition[J]. International journal of computer vision, 2013, 104(2): 154-171.

The code for the segmentation is from:
http://cs.brown.edu/people/pfelzens/segment/

