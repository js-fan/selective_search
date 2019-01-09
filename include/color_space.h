#ifndef COLOR_SPACE_H_
#define COLOR_SPACE_H_

#include <opencv2/opencv.hpp>

using uchar = unsigned char;

void to_hsv(const cv::Mat &input, cv::Mat &output);
void to_lab(const cv::Mat &input, cv::Mat &output);
void to_rgi(const cv::Mat &input, cv::Mat &output);
void to_hue(const cv::Mat &input, cv::Mat &output);
void to_gray(const cv::Mat &input, cv::Mat &output);

#endif
