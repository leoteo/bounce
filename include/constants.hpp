/*
 * constants.hpp
 *
 * Defines physical constants and conversion factors.
 */
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


#include "types.hpp"
using namespace types;

#define _USE_MATH_DEFINES
#include <math.h>

namespace constants {

    const real_t kB = 0.0083144621;   /* Boltzmann constant [kJ/mol/K] */

    const real_t mvsq = 1.000043E+4;   /* a.m.u.*(Angstrom/fs)**2 => kJ/mol */ 

}

#endif
