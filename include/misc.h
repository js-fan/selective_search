#ifndef MISC_H_
#define MISC_H_

#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <cstdlib>


// vector
using Vecf  = std::vector<float>;
using Vecf2 = std::vector<Vecf>;
using Veci  = std::vector<int>;
using Veci2 = std::vector<Veci>;
using Vecb  = std::vector<bool>;
using Vecb2 = std::vector<Vecb>;


// tedious operators
template <class Container> Container operator+(const Container &cont, const typename Container::value_type &elem);
template <class Container> Container operator+(const typename Container::value_type &elem, const Container &cont);
template <class Container> Container &operator+=(Container &cont, const typename Container::value_type &elem);
template <class Container> Container operator+(const Container &cont0, const Container &cont1);
template <class Container> Container &operator+=(Container &cont0, const Container &cont1);
template <class Container> Container operator*(const Container &cont, const typename Container::value_type &elem);
template <class Container> Container operator*(const typename Container::value_type &elem, const Container &cont);
template <class Container> Container &operator*=(Container &cont, const typename Container::value_type &elem);
template <class Container> Container operator/(const Container &cont, const typename Container::value_type &elem);
template <class Container> Container operator/(const typename Container::value_type &elem, const Container &cont);
template <class Container> Container &operator/=(Container &cont, const typename Container::value_type &elem);


// +
template <class Container>
Container operator+(const Container &cont, const typename Container::value_type &elem) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem0 + elem;});
    return res;
}


template <class Container>
Container operator+(const typename Container::value_type &elem, const Container &cont) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem0 + elem;});
    return res;
}

template <class Container>
Container &operator+=(Container &cont, const typename Container::value_type &elem) {
    std::transform(cont.cbegin(), cont.cend(), cont.begin(),
                   [&elem] (typename Container::value_type elem0) {return elem0 + elem;});
    return cont;
}

template <class Container>
Container operator+(const Container &cont0, const Container &cont1) {
    Container res;
    using elem_type = typename Container::value_type;
    std::transform(cont0.cbegin(), cont0.cend(), cont1.cbegin(), std::back_inserter(res),
                   [] (elem_type elem0, elem_type elem1) {return elem0 + elem1;});
    return res;
}

template <class Container>
Container &operator+=(Container &cont0, const Container &cont1) {
    using elem_type = typename Container::value_type;
    std::transform(cont0.cbegin(), cont0.cend(), cont1.cbegin(), cont0.begin(),
                   [] (elem_type elem0, elem_type elem1) {return elem0 + elem1;});
    return cont0;
}


// *
template <class Container>
Container operator*(const Container &cont, const typename Container::value_type &elem) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem0 * elem;});
    return res;
}

template <class Container>
Container operator*(const typename Container::value_type &elem, const Container &cont) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem * elem0;});
    return res;
}

template <class Container>
Container &operator*=(Container &cont, const typename Container::value_type &elem) {
    std::transform(cont.begin(), cont.end(), cont.begin(),
                   [&elem] (typename Container::value_type elem0) {return elem0 * elem;});
    return cont;
}

// /
template <class Container>
Container operator/(const Container &cont, const typename Container::value_type &elem) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem0 / elem;});
    return res;
}

template <class Container>
Container operator/(const typename Container::value_type &elem, const Container &cont) {
    Container res;
    std::transform(cont.cbegin(), cont.cend(), std::back_inserter(res),
                   [&elem] (typename Container::value_type elem0) {return elem / elem0;});
    return res;
}

template <class Container>
Container &operator/=(Container &cont, const typename Container::value_type &elem) {
    std::transform(cont.begin(), cont.end(), cont.begin(),
                   [&elem] (typename Container::value_type elem0) {return elem0 / elem;});
    return cont;
}


// algorithm
/*
template <class InputIt> 
typename std::iterator_traits<InputIt>::value_type minimum_sum(InputIt bg0, InputIt ed0, InputIt bg1) {
    typename std::iterator_traits<InputIt>::value_type res{};
    while (bg0 != ed0)
        res += std::min(*bg0++, *bg1++);
    return res;
}
*/

template <class InputIt0, class InputIt1, class BinaryFunc>
BinaryFunc for_each(InputIt0 bg0, InputIt0 ed0, InputIt1 bg1, BinaryFunc func) {
    while (bg0 != ed0)
        func(*bg0++, *bg1++);
    return func;
}


// ostream
template <class T> 
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    os << "[";
    for (auto it : vec)
        os << it << ", ";
    os << "]";
    return os;
} 


// Box and Region
struct Box {
    Box(Veci v) : x0(v[0]), y0(v[1]), x1(v[2]), y1(v[3]) {}
    Box(Vecf v) : x0(v[0]), y0(v[1]), x1(v[2]), y1(v[3]) {}
    Box(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1) {}
    Box(const Box &box0, const Box &box1) {
        x0 = std::min(box0.x0, box1.x0);
        y0 = std::min(box0.y0, box1.y0);
        x1 = std::max(box0.x1, box1.x1);
        y1 = std::max(box0.y1, box1.y1);
    }
    inline int size() {return (x1 - x0 + 1) * (y1 - y0 + 1);}
    int x0, y0, x1, y1;
};

struct Region {
    Region(int _id, int _size, int _isize, Box _box, Vecf _hc, Vecf _ht) : id(_id), size(_size),
        image_size(_isize), box(_box), hist_color(_hc), hist_texture(_ht) {}

    int id, size, image_size;
    Box box;
    Vecf hist_color, hist_texture;
};

std::ostream &operator<<(std::ostream &os, const Region &r);

// random
inline float mrand() {return float(std::rand()) / RAND_MAX;}

#endif
