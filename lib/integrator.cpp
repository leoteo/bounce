/*
 * integrator.cpp
 */

#include "integrator.hpp"
#include "types.hpp"
using namespace types;

void VelocityVerlet::step(State& s){
        size_t i, r;
        size_t N = s.N;

        /* 1/2 velocity step plus 1 position step */
        for(i=0; i < N; ++i){
            for(r=0; r < 3; ++r){
                s.v(i,r) += 0.5 * dt / mvsq2e * s.f(i,r) / s.m[i];
                s.x(i,r) += dt * s.v(i,r);
            }
        }

        /* compute forces */
        s.f.zero();
        s.U = 0.0;
        size_t NI = s.forces.size();
        for(i=0; i < NI; ++i){
              forces[i].add(State& s);
          }

        /* 1/2 velocity step */
        for(i=0; i < N; ++i){
            for(r=0; r < 3; ++r){
                s.v(i,r) += 0.5 * dt / mvsq2e * s.f(i,r) / s.m(i);
            }
        }

}
