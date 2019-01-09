#include "selective_search.h"

typedef unsigned char uchar;

extern "C" {

    int *selective_search_src(const char *image_src, int mode, int *num_box, int min_box) {
        std::vector<Box> _boxes;
        *num_box = selective_search(image_src, Mode(mode), _boxes, min_box);

        int *boxes = new int[*num_box * 4];
        for (int i = 0; i < *num_box; ++i) {
            int *b = boxes + i * 4;
            b[0] = _boxes[i].x0;
            b[1] = _boxes[i].y0;
            b[2] = _boxes[i].x1;
            b[3] = _boxes[i].y1;
        }

        return boxes;
    }

    int *selective_search_mat(uchar *data, int height, int width,
                              int mode, int *num_box, int min_box) {
        cv::Mat image(height, width, CV_8UC3, data);
        std::vector<Box> _boxes;

        *num_box = selective_search(image, Mode(mode), _boxes, min_box);

        int *boxes = new int[*num_box * 4];
        for (int i = 0; i < *num_box; ++i) {
            int *b = boxes + i * 4;
            b[0] = _boxes[i].x0;
            b[1] = _boxes[i].y0;
            b[2] = _boxes[i].x1;
            b[3] = _boxes[i].y1;
        }

        return boxes;
    }

    void clear(int *boxes) {
        delete[] boxes;
    }
}
