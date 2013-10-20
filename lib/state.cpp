/*
 * state.cpp
 */

#include "state.hpp"
#include "constants.hpp"
#include "random.hpp"

State::State(size_t N, VecVec3d x, vec_t<real_t> m, vec_t<Force*> forces, 
             vec_t<real_t> cell, real_t t0) :
    x(x),
    v(VecVec3d(N,0.0)),
    f(VecVec3d(N, 0.0)),
    forces(forces),
    m(m),
    ePot(0.0),
    eKin(0.0),
    eTot(0.0),
    N(N), 
    cell(cell) {

    /* initialize forces */
    size_t nf = forces.size();
    for(size_t i=0; i < nf; ++i){
          forces(i)->add(this);
    }

    /* initialize velocities */
    draw_v(t0);

}


inline void State::draw_v(real_t t0) {
    NormalDistribution d = NormalDistribution();

    real_t tmp;
    for(size_t i=0; i < N; ++i){
        tmp = sqrt(constants::kB * t0 / (m(i) * constants::mvsq));
        for(size_t r=0; r<3; ++r){
            v(i,r) = d.draw() * tmp;
            
        }
    }

}
