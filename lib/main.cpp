#include "types.hpp"
using namespace types;
#include "parsers.hpp"
#include "bounce.hpp"

int main(int ac, char* av[]) {

    InputParser p(ac, av);

    Bounce mycalc(p);

    mycalc.calculate();

    return 0;
}

