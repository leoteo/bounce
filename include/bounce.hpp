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
        InputParser in;
        Integrator *integrator;
        State state;
        OutputParser out;

    public:
        Bounce(const InputParser &p): in(p) { in.init(integrator, state, out); }
        ~Bounce(){ delete integrator; }
        void calculate();

};

#endif
