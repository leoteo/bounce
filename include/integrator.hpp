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
        Integrator(real_t dt, size_t nstep, integrator::type t) :
            dt(dt), nstep(nstep), step(0), type(t) {}
        virtual ~Integrator() =0;
        virtual void dostep(State &s) =0;
        real_t t() { return step*dt; }
        bool done() { return step <= nstep; }
        size_t get_step() const { return step; }; 
};

class VelocityVerlet : public Integrator {
    public:
        VelocityVerlet(real_t dt, size_t nstep) : 
            Integrator(dt,nstep, integrator::VelocityVerlet) {}
        ~VelocityVerlet() {}
        void dostep(State &s);
};


#endif
