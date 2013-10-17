/*
 * bounce.cpp
 */

#include "bounce.hpp"
#include "types.hpp"
using namespace types;
#include "Timer.hpp"

void Bounce::init(const InputParser &p){
    in = p;
    in.init(integrator, state);

    out = OututParser();
}

void Bounce::calculate(){

    Timer timer();

    std::cout << "Starting calculation.\n";
    timer.print();

    while( ! integrator.done() ){
        integrator.step(state);
        
        out.write(integrator, state);
    }


    std::cout << "Done with calculation.\n";
    timer.print();
}
