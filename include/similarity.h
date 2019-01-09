#ifndef SIMILARITY_H_
#define SIMILARITY_H_

#include "misc.h"


inline Vecf::value_type sim_color(const Region &r0, const Region &r1) {
    Vecf::value_type val{0};
    for_each(r0.hist_color.cbegin(), r0.hist_color.cend(), r1.hist_color.cbegin(),
             [&] (Vecf::value_type v0, Vecf::value_type v1) {val += std::min(v0, v1);});
    return val;
}

inline Vecf::value_type sim_texture(const Region &r0, const Region &r1) {
    Vecf::value_type val{0};
    for_each(r0.hist_texture.cbegin(), r0.hist_texture.cend(), r1.hist_texture.cbegin(),
             [&] (Vecf::value_type v0, Vecf::value_type v1) {val += std::min(v0, v1);});
    return val;
}

inline Vecf::value_type sim_size(const Region &r0, const Region &r1) {
    return 1. - Vecf::value_type(r0.size + r1.size) / r0.image_size;
}

inline Vecf::value_type sim_fill(const Region &r0, const Region &r1) {
    return 1. - Vecf::value_type(Box(r0.box, r1.box).size() - r0.size - r1.size) / r0.image_size;
}



#endif
