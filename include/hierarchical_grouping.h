#ifndef HIERARCHICAL_GROUPING_H_
#define HIERARCHICAL_GROUPING_H_

#include <opencv2/opencv.hpp>
#include "histogram.h"
#include "similarity.h"
#include "misc.h"

void init_regions(const cv::Mat &image, const cv::Mat &segment_comp, int num_ccs,
                  std::vector<Region> &regions, Vecb2 &neighbours);

void init_similarities(const std::vector<Region> &regions, const Vecb2 &neighbours,
                       std::vector<Vecf2> &sim_mats);

void hierarchical_grouping(std::vector<Region> &regions, Vecb2 &neighbours,
                           std::vector<Vecf2> &sim_mats, std::vector<Box> &boxes);
#endif
