#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <opencv2/opencv.hpp>
#include "misc.h"


constexpr int NUM_C_BIN = 25;
constexpr int NUM_T_BIN = 10;
constexpr int NUM_O_BIN = 8;


void init_color_histogram(const cv::Mat &image, const cv::Mat &image_comp, int num_ccs, Vecf2 &histogram_list);

void init_texture_histogram(const cv::Mat &image, const cv::Mat &image_comp, int num_ccs, Vecf2 &histogram_list);

#endif
