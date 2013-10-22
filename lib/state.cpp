/*
 * state.cpp
 */

#include "state.hpp"
#include "constants.hpp"
#include "random.hpp"

State::State(
        size_t N, 
        VecVec3d x, 
        vec_t<real_t> m, 
        vec_t<string_t> symbols,
        vec_t<Force*> forces,
        vec_t<real_t> cell, 
        real_t t0) :
    N(N), 
    x(x),
    m(m),
    symbols(symbols),
    v(VecVec3d(N,0.0)),
    f(VecVec3d(N, 0.0)),
    forces(forces),
    ePot(0.0),
    eKin(0.0),
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

    // Draw velocities from normal distribution of appropiate sigma
    real_t scalef;
    for(size_t i=0; i < N; ++i){
        scalef = sqrt(constants::kB * t0 / (m(i) * constants::mvsq));
        for(size_t r=0; r<3; ++r){
            v(i,r) = d.draw() * scalef;
            eKin += 0.5*m(i)*v(i,r)*v(i,r) * constants::mvsq;
        }
    }

    // Impose initial temperature t0 exactly
    real_t t = 2.0 * eKin / ((3.0 * N - 3.0) * constants::kB);
    scalef = sqrt(t0/t);

    eKin *= scalef*scalef;
    for(size_t i=0; i < N; ++i)
        for(size_t r=0; r<3; ++r)
            v(i,r) *= scalef;

}

VecVec3d State::pTot() const {
    VecVec3d pTot = VecVec3d(1, 0.0);
    for(size_t i=0; i < N; ++i){
        for(size_t r=0; r<3; ++r){
            pTot(0,r) += m(i) * v(i,r);
        }
    }
    return pTot;
}

VecVec3d State::lTot() const {
    VecVec3d lTot = VecVec3d(1,0.0);
    for(size_t i=0; i < N; ++i){
        lTot += m(i) * cross(x, i, v, i);
    }
    return lTot;
}

