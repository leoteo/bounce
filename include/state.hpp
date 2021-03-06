/*
 * state.hpp 
 */
#ifndef STATE_HPP
#define STATE_HPP

#include "types.hpp"
using namespace types;
#include "forces.hpp"

class State {
    public:
        size_t N;         /** Number of particles */
        VecVec3d x;
        VecVec3d v;
        VecVec3d f;

        vec_t<Force*> forces;
        vec_t<real_t> m;              /** Masses of particles */
        vec_t< string_t > symbols;    /** Symbols for particles */

        real_t ePot;      /** Potential energy */
        real_t eKin;      /** Kinetic energy */
        real_t temp;      /** Temperature */

        vec_t< real_t > cell;  /** only rectangular cell supported */

        State() {}
        State(size_t N) : 
            N(N),
            x(VecVec3d(N)),
            v(VecVec3d(N)),
            f(VecVec3d(N)),
            m(vec_t<real_t>(N, 1.0)),
            ePot(0.0),
            eKin(0.0),
            temp(0.0),
            cell(vec_t<real_t>(3,1.0)) 
                {}
        State(size_t N,                /** to be used from xyz file */
              VecVec3d x, 
              vec_t<real_t> m, 
              vec_t<string_t> symbols,
              vec_t<Force*> forces, 
              vec_t<real_t> cell,
              real_t t0); 
        ~State(){ for(size_t i; i<forces.size(); ++i) delete forces(i); }

        inline real_t dist(size_t i, size_t j) const;
        inline real_t pbc(real_t d, size_t dim) const;
        inline void draw_v(real_t t0);

        inline real_t eTot() const { return eKin + ePot; }
        VecVec3d pTot() const;
        VecVec3d lTot() const;
        
};

#include <math.h>

inline real_t State::dist(size_t i, size_t j) const {
    real_t d, sum = 0.0;
    for(d=0; d<3; ++d) 
        sum += pbc( x(j,d) - x(i,d), d )
             * pbc( x(j,d) - x(i,d), d );
    return sqrt(sum);
}

inline real_t State::pbc(real_t d, size_t dim) const {
    while( d >  0.5 * cell(dim)) d -= cell(dim);
    while( d < -0.5 * cell(dim)) d += cell(dim);
    return d;
}

#endif
