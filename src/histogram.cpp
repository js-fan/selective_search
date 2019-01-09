#include "histogram.h"


void init_color_histogram(const cv::Mat &image, const cv::Mat &segment_comp, int num_ccs,
                          Vecf2 &histogram_list) {
    // init container
    histogram_list.clear();
    std::fill_n(std::back_inserter<Vecf2>(histogram_list), num_ccs, Vecf(NUM_C_BIN * 3, 0));
    Vecf comp_sizes(num_ccs, 0);

    // histogram
    constexpr float bin_eps = (255. + 1e-5) / NUM_C_BIN;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            const cv::Vec3b pxl = image.at<cv::Vec3b>(i, j);
            const int idx = segment_comp.at<int>(i, j);
            for (int k : {0, 1, 2})
                ++histogram_list[idx][NUM_C_BIN * k + int(pxl.val[k] / bin_eps)];
            ++comp_sizes[idx];
        }
    }

    // L1 norm
    for_each(histogram_list.begin(), histogram_list.end(), comp_sizes.cbegin(),
             [] (Vecf &vec, Vecf::value_type size) {vec /= size;});
}


void init_texture_histogram(const cv::Mat &image, const cv::Mat &segment_comp,
                            int num_ccs, Vecf2 &histogram_list) {
    // init container
    histogram_list.clear();
    std::fill_n(std::back_inserter<Vecf2>(histogram_list), num_ccs,
                Vecf(NUM_T_BIN * NUM_O_BIN * 3, 0));
    Vecf comp_sizes(num_ccs, 0);

    // gradient
    cv::Mat _image;
    cv::GaussianBlur(image, _image, cv::Size(), 1, 0);
    _image.convertTo(_image, CV_32F, 1./255);

    cv::Mat kernel = cv::Mat::zeros(1, 3, CV_32FC1);
    kernel.at<float>(0) = -1;
    kernel.at<float>(2) = 1;

    cv::Mat gx, gy;
    cv::filter2D(_image, gx, CV_32F, kernel);
    cv::filter2D(_image, gy, CV_32F, kernel.t());

    // histogram
    constexpr float ang_eps = (M_PI * 2. + 1e-5) / NUM_O_BIN;
    constexpr float val_eps = (255. + 1e-5) / NUM_T_BIN;

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            const cv::Vec3b pxl = image.at<cv::Vec3b>(i, j);
            const cv::Vec3f _gx = gx.at<cv::Vec3f>(i, j);
            const cv::Vec3f _gy = gy.at<cv::Vec3f>(i, j);
            const int idx = segment_comp.at<int>(i, j);
            for (int k : {0, 1, 2}) {
                const int idx_ang = int((std::atan2(_gy.val[k], _gx.val[k]) + M_PI) / ang_eps);
                const int idx_val = int(pxl.val[k] / val_eps);
                ++histogram_list[idx][NUM_O_BIN*NUM_T_BIN*k + NUM_T_BIN*idx_ang + idx_val];
            }
            ++comp_sizes[idx];
        }
    }

    // L1 norm
    for_each(histogram_list.begin(), histogram_list.end(), comp_sizes.cbegin(),
             [] (Vecf &vec, Vecf::value_type size) {vec /= size;});
}

