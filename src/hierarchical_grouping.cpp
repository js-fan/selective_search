#include "hierarchical_grouping.h"


void init_regions(const cv::Mat &image, const cv::Mat &segment_comp, int num_ccs,
                  std::vector<Region> &regions, Vecb2 &neighbours) {
    // init containers
    regions.clear();
    neighbours.clear();
    std::fill_n(std::back_inserter(neighbours), num_ccs, Vecb(num_ccs, false));

    int height = image.rows;
    int width  = image.cols;
    
    Veci comp_sizes(num_ccs, 0);
    Veci2 minmax(num_ccs, Veci{width-1, height-1, 0, 0});

    for (int x = 0; x < width; ++x) {
        int prev_idx = segment_comp.at<int>(0, x);
        for (int y = 0; y < height; ++y) {
            int curr_idx = segment_comp.at<int>(y, x);

            ++comp_sizes[curr_idx];
            if (x < minmax[curr_idx][0])
                minmax[curr_idx][0] = x;
            if (y < minmax[curr_idx][1])
                minmax[curr_idx][1] = y;
            if (x > minmax[curr_idx][2])
                minmax[curr_idx][2] = x;
            if (y > minmax[curr_idx][3])
                minmax[curr_idx][3] = y;

            neighbours[prev_idx][curr_idx] = true;
            neighbours[curr_idx][prev_idx] = true;

            if (x > 0) {
                int left_idx = segment_comp.at<int>(y, x - 1);
                neighbours[left_idx][curr_idx] = true;
                neighbours[curr_idx][left_idx] = true;
            }

            prev_idx = curr_idx;
        }
    }

    for (int i = 0; i < num_ccs; ++i)
        neighbours[i][i] = false;

    // histograms
    Vecf2 hist_color, hist_texture; 
    init_color_histogram(image, segment_comp, num_ccs, hist_color);
    init_texture_histogram(image, segment_comp, num_ccs, hist_texture);

    // collect regions
    int image_size = width * height;
    for (int i = 0; i < num_ccs; ++i)
        regions.emplace_back(i, comp_sizes[i], image_size,
                Box(minmax[i]), hist_color[i], hist_texture[i]);
}


void init_similarities(const std::vector<Region> &regions, const Vecb2 &neighbours,
                       std::vector<Vecf2> &sim_mats) {
    const int num_ccs = regions.size();
    sim_mats.clear();
    std::fill_n(std::back_inserter<std::vector<Vecf2>>(sim_mats), 4, Vecf2(num_ccs, Vecf(num_ccs, 0)));

    for (int i = 0; i < num_ccs; ++i) {
        for (int j = i + 1; j < num_ccs; ++j) {
            if (!neighbours[i][j])
                continue;

            sim_mats[0][i][j] = sim_color(regions[i], regions[j]);
            sim_mats[1][i][j] = sim_texture(regions[i], regions[j]);
            sim_mats[2][i][j] = sim_size(regions[i], regions[j]);
            sim_mats[3][i][j] = sim_fill(regions[i], regions[j]);

            sim_mats[0][j][i] = sim_mats[0][i][j];
            sim_mats[1][j][i] = sim_mats[1][i][j];
            sim_mats[2][j][i] = sim_mats[2][i][j];
            sim_mats[3][j][i] = sim_mats[3][i][j];
        }
    }
}


bool find_max_sim_id(const Vecb2 &neighbours, const std::vector<Vecf2> &sim_mats, int &id_min, int &id_max) {
    float max{0};
    int num_ccs = neighbours.size();

    for (int i = 0; i < num_ccs; ++i) {
        for (int j = i + 1; j < num_ccs; ++j) {
            if (!neighbours[i][j])
                continue;
            float val{0};
            std::for_each(sim_mats.cbegin(), sim_mats.cend(), [&val, i, j] (const Vecf2 &mat) {val += mat[i][j];});
            if (val > max) {
                max = val;
                id_min = i;
                id_max = j;
            }
        }
    }
    return max > 0;
}


void hierarchical_grouping(std::vector<Region> &r, Vecb2 &neighbours, std::vector<Vecf2> &sim_mats,
                           std::vector<Box> &boxes) {
    for (Region &_r : r)
        boxes.push_back(_r.box);

    const int num_ccs = r.size();


    for (int k = 0; k < num_ccs-1; ++k) {
        int i, j;

        /* Debug:
        float sum{0};
        std::for_each(neighbours.cbegin(), neighbours.cend(), [&sum] (Vecb neigh_line) {
                sum += std::accumulate(neigh_line.cbegin(), neigh_line.cend(), float{0});});
        std::cout << "sum = " << sum << std::endl;
        */

        if (find_max_sim_id(neighbours, sim_mats, i, j))  {
            // merge j's neighbours to i
            for (int n = 0; n < num_ccs; ++n) {
                if (n == i || n == j)
                    continue;
                if (neighbours[j][n]) {
                    neighbours[i][n] = true;
                    neighbours[n][i] = true;
                    neighbours[j][n] = false;
                    neighbours[n][j] = false;
                }
                neighbours[i][j] = false;
                neighbours[j][i] = false;
            }

            // update i's region info
            r[i].hist_color = (r[i].hist_color * r[i].size + r[j].hist_color * r[j].size) / (r[i].size + r[j].size);
            r[i].hist_texture = (r[i].hist_texture * r[i].size + r[j].hist_texture * r[j].size) / (r[i].size + r[j].size);
            r[i].size = r[i].size + r[j].size;
            r[i].box = Box(r[i].box, r[j].box);
            r[j].id = -1;

            // update sim_mats
            for (int n = 0; n < num_ccs; ++n) {
                if (neighbours[i][n]) {
                    sim_mats[0][i][n] = sim_color(r[i], r[n]);
                    sim_mats[1][i][n] = sim_texture(r[i], r[n]);
                    sim_mats[2][i][n] = sim_size(r[i], r[n]);
                    sim_mats[3][i][n] = sim_fill(r[i], r[n]);

                    sim_mats[0][n][i] = sim_mats[0][i][n];
                    sim_mats[1][n][i] = sim_mats[1][i][n];
                    sim_mats[2][n][i] = sim_mats[2][i][n];
                    sim_mats[3][n][i] = sim_mats[3][i][n];
                }
            }

            boxes.push_back(r[i].box);
        } else {
            std::cerr << "k = " << k << std::endl;
            break;
        }
    }

}
