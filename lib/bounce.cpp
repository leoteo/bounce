/*
 * bounce.cpp
 */

#include "bounce.hpp"
#include "types.hpp"
using namespace types;
#ifdef TIME
#include "time.hpp"
#endif

void Bounce::calculate(){

    std::cout << "Starting calculation.\n";

#ifdef TIME
    Timer timer;
    Timer steptimer;
    timer.print();
#endif
    while( ! integrator->is_done() ){
#ifdef TIME
        steptimer.start();
#endif
        integrator->do_step(state);
#ifdef TIME
        steptimer.print();
#endif
        
        out->write(integrator, state);
    }


    std::cout << "Done with calculation.\n";
#ifdef TIME
    timer.print();
#endif
}
