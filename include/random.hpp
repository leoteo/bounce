/*
 * random.hpp
 *
 * Random number generator for normal distributions  
 */
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "types.hpp"
using namespace types;
#include "constants.hpp"
#include <stdlib.h>

/**
 * Use Box-Muller algorithm to generate a pair of
 * normally distributed variables from
 * uniformly distributed variables.
 */
class NormalDistribution {
    private:
        real_t t1;
        real_t t2;
        bool update;
        real_t sigma;
    public:
        NormalDistribution(): t1(drand48()), t2(drand48()), 
            update(false), sigma(1.0) {}
        NormalDistribution(real_t sigma): t1(drand48()), t2(drand48()), 
            update(false), sigma(sigma) {}
        inline real_t draw();
};

inline real_t NormalDistribution::draw(){
    real_t v;

    if (! update) {
        v = sigma * sqrt(-2.0 * log(t1)) * cos(2 * M_PI * t2);
        update = true;
    } else {
        v = sigma * sqrt(-2.0 * log(t1)) * sin(2 * M_PI * t2);
        t1 = drand48();
        t2 = drand48();
        update = false;
    }

    return v;
}


#endif
