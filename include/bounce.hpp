/*
 * bounce.hpp
 */
#ifndef BOUNCE_HPP
#define BOUNCE_HPP

#include "types.hpp"
using namespace types;
#include "parsers.hpp"
#include "integrator.hpp"
#include "state.hpp"


/**
 * 'Bounce' mother class
 *
 * Holds all informations on the system,
 * i.e. the integrator as well as the state.
 */
class Bounce {
    private:
        const InputParser* in;
        Integrator* integrator;
        State* state;
        OutputParser* out;

    public:
        Bounce(const InputParser &p): 
            in(&p), 
            integrator(in->get_integrator()),
            state(in->get_state()),
            out(in->get_outputparser())
                {}
        ~Bounce(){ delete in; delete integrator; delete state; delete out; }
        void calculate();

};

#endif
