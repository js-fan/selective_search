#include "selective_search.h"

int selective_search(const cv::Mat &image, Color c, float sigma, float k, int min_box, std::vector<Box> &boxes) {
    cv::Mat _image;
    switch (c) {
        case Color::hsv: to_hsv(image, _image); break;
        case Color::lab: to_lab(image, _image); break;
        case Color::rgi: to_rgi(image, _image); break;
        case Color::hue: to_hue(image, _image); break;
        case Color::gray: to_gray(image, _image); break;
        case Color::rgb: image.copyTo(_image); break;
        default: std::cerr << "Error Color value" << std::endl;
    }

    cv::Mat segment_comp; 
    int num_ccs = segment(_image, segment_comp, sigma, k, k);

    std::vector<Region> regions;
    Vecb2 neighbours;
    init_regions(_image, segment_comp, num_ccs, regions, neighbours);

    std::vector<Vecf2> sim_mats;
    init_similarities(regions, neighbours, sim_mats);

    hierarchical_grouping(regions, neighbours, sim_mats, boxes);

    boxes.erase(std::remove_if(boxes.begin(), boxes.end(), [min_box](Box _box) -> bool {
                return std::min(_box.x1 - _box.x0, _box.y1 - _box.y0) < min_box - 1;}),
                boxes.end());
    return boxes.size();
}


int selective_search(const cv::Mat &image, std::vector<Color> cs, float sigma, std::vector<float> ks,
                     int min_box, std::vector<Box> &boxes) {
    std::vector<std::pair<Color, float>> params;
    for (Color c : cs)
        for (float k : ks)
            params.emplace_back(c, k);

    std::vector<std::vector<Box>> tmp_boxes(params.size());

    #ifdef use_openmp_
    #pragma omp parallel for
    #endif
    for (int i = 0; i < params.size(); ++i) {
        Color c = params[i].first;
        float k = params[i].second;
        selective_search(image, c, sigma, k, min_box, tmp_boxes[i]);
    }

    for (auto _boxes : tmp_boxes)
        std::move(_boxes.begin(), _boxes.end(), std::back_inserter(boxes));
    
    boxes.erase(std::unique(boxes.begin(), boxes.end(), [](Box b0, Box b1) {
                return (b0.x0 == b1.x0) && (b0.x1 == b1.x1) && (b0.y0 == b1.y0) && (b0.y1 == b1.y1);}),
                boxes.end());
    return boxes.size();
}


int selective_search(const cv::Mat &image, Mode mode, std::vector<Box> &boxes, int min_box) {
    std::vector<float> ks;
    std::vector<Color> cs;

    if (mode == Mode::fast) {
        ks = {50, 100};    
        cs = {Color::hsv, Color::lab};
    } else if (mode == Mode::quality) {
        ks = {50, 100, 150, 300};
        cs = {Color::hsv, Color::lab, Color::rgi, Color::hue, Color::gray};
    }

    selective_search(image, cs, 0.8, ks, min_box, boxes);
    return boxes.size();
}


int selective_search(const char *image_src, Mode mode, std::vector<Box> &boxes, int min_box) {
    cv::Mat image = cv::imread(image_src);
    return selective_search(image, mode, boxes, min_box);
}


void draw_boxes(cv::Mat &image, const std::vector<Box> boxes, bool random_color) {
    cv::Scalar color;
    for (Box box : boxes) {
        if (random_color)
            color = cv::Scalar(int(mrand() * 255.999), int(mrand() * 255.999), int(mrand() * 255.999));
        else
            color = cv::Scalar(0, 0, 255);
        cv::rectangle(image, cv::Point(box.x0, box.y0), cv::Point(box.x1, box.y1), color);
    }
}
