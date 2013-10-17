/*
 * integrator.hpp
 */
#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "types.hpp"
using namespace types;
#include "state.hpp"
#include "forces.hpp"

namespace integrator {
    enum type { VelocityVerlet = 1 };
}
    

class Integrator {
    protected:
        real_t dt;
        size_t nstep;
        size_t step;
        integrator::type type;
    public:
        virtual void integrate(State &s) =0;
        virtual void step(State &s) =0;
        virtual real_t t() { return step*dt; }
        inline virtual bool done() { return step <= nstep; }
}

class VelocityVerlet : public Integrator {
    public:
        void step(State &s);
        VelocityVerlet(real_t dt, size_t nstep) : 
            dt(dt), nstep(nstep), step(0), 
            type(VelocityVerlet) {};
}


#endif
