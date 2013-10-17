/*
 * state.cpp
 */

#include "state.hpp"
#include "types.hpp"
using namespace types;

#include <math.h>

inline real_t State::dist(size_t i, size_t j) const {
    real_t d, sum = 0.0;
    for(d=0; d<3; ++d) 
        sum += pbc( x(j,d) - x(i,d), d )
             * pbc( x(j,d) - x(i,d), d );
    return sqrt(sum);
}

inline real_t State::pbc(real_t d, size_t dim) const {
    return std::fmod(d, cell(dim)*0.5);
}


