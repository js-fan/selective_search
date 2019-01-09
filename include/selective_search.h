#ifndef SELECTIVE_SEARCH_H_
#define SELECTIVE_SEARCH_H_

#include "hierarchical_grouping.h"
#include "color_space.h"
#include "segment.h"

#ifdef use_openmp_
#include <omp.h>
#endif


enum Color {
    hsv, lab, rgi, hue, gray, rgb
};

enum Mode {
    fast = 0,
    quality = 1
};


int selective_search(const cv::Mat &image, Color c, float sigma, float k, int min_box, std::vector<Box> &boxes);

int selective_search(const cv::Mat &image, std::vector<Color> cs, float sigma, std::vector<float> ks,
                     int min_box, std::vector<Box> &boxes);

int selective_search(const cv::Mat &image, Mode mode, std::vector<Box> &boxes, int min_box=20);

int selective_search(const char *image_src, Mode mode, std::vector<Box> &boxes, int min_box=20);


void draw_boxes(cv::Mat &image, const std::vector<Box> boxes, bool random_color=true);

#endif
