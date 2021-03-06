#include "selective_search.h"


int main() {
    cv::Mat image = cv::imread("./demo/skimage_astronaut.jpg");
    std::vector<Box> boxes;

    // int num_box = selective_search(image, std::vector<Color>{Color::hsv, Color::lab}, 0.8, std::vector<float>{50, 100, 150}, 20, boxes);
    int num_box = selective_search(image, Mode::fast, boxes);
    std::cout << "Obtained " << num_box << " boxes." << std::endl;

    draw_boxes(image, boxes);
    cv::imwrite("./demo/box.jpg", image);
    std::cout << "Saved as ./demo/box.jpg" << std::endl;
    return 0;
}
