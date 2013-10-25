/*
 * integrator.cpp
 */

#include "integrator.hpp"
#include "types.hpp"
using namespace types;
#include "constants.hpp"

#ifdef TIME
#include "time.hpp"
#endif

Integrator::~Integrator() {}

void VelocityVerlet::do_step(State* s){
        size_t i, r;
        size_t N = s->N;

        /* 1/2 velocity step plus 1 position step */
        for(i=0; i < N; ++i){
            for(r=0; r < 3; ++r){
                s->v(i,r) += 0.5 * dt  * s->f(i,r) / ( constants::mvsq * s->m(i));
                s->x(i,r) += dt * s->v(i,r);
            }
        }
#ifdef TIME
        Timer tforce;
        tforce.start();
#endif

        /* compute forces */
        s->f.zero();
        s->ePot = 0.0;
        size_t nf = s->forces.size();
        for(i=0; i < nf; ++i){
              s->forces(i)->add(s);
          }
#ifdef TIME
        std::cout << "Forces: " << tforce.read() << "s\n";
#endif 

        /* 1/2 velocity step */
        s->eKin = 0.0;
        for(i=0; i < N; ++i){
            for(r=0; r < 3; ++r){
                s->v(i,r) += 0.5 * dt / constants::mvsq * s->f(i,r) / s->m(i);
                s->eKin   += 0.5 * s->m(i) * s->v(i,r) * s->v(i,r) * constants::mvsq;
             }
        }

        s->temp = 2.0 * s->eKin/(3 * s->N - 3)/constants::kB;

        ++step;
}

