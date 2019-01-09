#include "misc.h"


std::ostream &operator<<(std::ostream &os, const Region &r) {
    os << "{ id = " << r.id << ", size = " << r.size << std::endl <<
          "  hist_color = " << r.hist_color << std::endl <<
          "  hist_texture = " << r.hist_texture << " }" << std::endl;
    return os;
}
