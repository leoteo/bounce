/*
 * bounce.hpp
 */
#ifndef BOUNCE_HPP
#define BOUNCE_HPP

#include "types.hpp"
namespace t = types;
#include "state.hpp"
#include "integrator.hpp"


/**
 * 'Bounce' mother class
 *
 * Holds all informations on the system,
 * i.e. the integrator as well as the state.
 */
class Bounce {
    private:
        InputParser in;
        Integrator integrator;
        State state;
        OutputParser out;

    public:
        Bounce() {}
        void init(const InputParser &p);
        void calculate(){}


#endif
