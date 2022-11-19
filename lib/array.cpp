#include "array.h"

#include <cstddef>
#include "math.h"

template<>
Array<bool> Array_builder<Array<bool>, bool>::build() {
    Array<bool> a(count_);
    int index = -1;
    for (bool b : list_) { // todo optimize?
        a[++index] = b;
    }
    return a;
}

Array<bool>::Array(size_t size) 
    :actual_size_(divide_round_up_int(size, static_cast<size_t>(CHAR_BIT))),
    ptr_(new unsigned char[actual_size_]), size_(size) 
{}

Array<bool>::Array(size_t size, bool value) :Array(size) {
    if (value)
        for (auto p = ptr_; p != ptr_ + actual_size_; *p = UCHAR_MAX, ++p);
    else
        for (auto p = ptr_; p != ptr_ + actual_size_; *p = 0, ++p);
}
