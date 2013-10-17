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
        VecVec3d x;
        VecVec3d v;
        VecVec3d f;

        vec_t<Force*> forces;
        vec_t<real_t> m;  /** Masses of particles */

        real_t ePot;      /** Potential energy */
        real_t eKin;      /** Kinetic energy */
        real_t eTot;      /** Total energy */
        size_t N;         /** Number of particles */

        vec_t< real_t > cell;  /** only rectangular cell supported */

        State() {}
        State(size_t N) : 
            x(VecVec3d(N)),
            v(VecVec3d(N)),
            f(VecVec3d(N)),
            m(vec_t<real_t>(N, 1.0)),
            ePot(0.0),
            eKin(0.0),
            eTot(0.0),
            N(N) 
                {}
        State(size_t N, VecVec3d x, VecVec3d v, vec_t<real_t> m, vec_t<Force*> forces) :
            x(x),
            v(v),
            f(VecVec3d(N)),
            forces(forces),
            m(m),
            ePot(0.0),
            eKin(0.0),
            eTot(0.0),
            N(N) 
                {}
        ~State(){ for(size_t i; i<forces.size(); ++i) delete forces(i); }

        inline real_t dist(size_t i, size_t j) const;
        inline real_t pbc(real_t d, size_t dim) const;
        
};


#endif
