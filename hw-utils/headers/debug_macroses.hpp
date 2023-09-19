#pragma once

#ifdef DEBUG
#include <iostream>
#define ARR_OUT_OF_RANGE_GUARD(arr_size, index) \
    if ((index) >= (arr_size) || (index) < 0) { \
        std::cout << "Error: Index our of range! index: " << (index) << "; array size: " << (arr_size) << ";" << std::endl; \
    }
#else
#define ARR_OUT_OF_RANGE_GUARD(arr_size, index)
#endif
