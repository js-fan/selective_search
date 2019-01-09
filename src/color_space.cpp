#include "color_space.h"


void to_hsv(const cv::Mat &input, cv::Mat &output) {
    cv::cvtColor(input, output, cv::COLOR_BGR2HSV);
}


void to_lab(const cv::Mat &input, cv::Mat &output) {
    cv::cvtColor(input, output, cv::COLOR_BGR2Lab);
}


void to_rgi(const cv::Mat &input, cv::Mat &output) {
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    input.copyTo(output);
    for (int x = 0; x < input.cols; ++x)
        for (int y = 0; y < input.rows; ++y)
            output.at<cv::Vec3b>(y, x).val[0] = gray.at<uchar>(y, x);
}


void to_hue(const cv::Mat &input, cv::Mat &output) {
    cv::cvtColor(input, output, cv::COLOR_BGR2HSV);
    for (int x = 0; x < input.cols; ++x) {
        for (int y = 0; y < input.rows; ++y) {
            uchar val = output.at<cv::Vec3b>(y, x).val[0];
            output.at<cv::Vec3b>(y, x).val[1] = val;
            output.at<cv::Vec3b>(y, x).val[2] = val;
        }
    }
}


void to_gray(const cv::Mat &input, cv::Mat &output) {
    cv::Mat gray;
    output.create(input.rows, input.cols, CV_8UC3);
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    for (int x = 0; x < input.cols; ++x) {
        for (int y = 0; y < input.rows; ++y) {
            uchar val = gray.at<uchar>(y, x);
            output.at<cv::Vec3b>(y, x).val[0] = val;
            output.at<cv::Vec3b>(y, x).val[1] = val;
            output.at<cv::Vec3b>(y, x).val[2] = val;
        }
    }
}

