/*
 * constants.cpp
 */

#include "constants.hpp"
#include "types.hpp"
using namespace types;

namespace constants {

    /** Convert atomic symbol to mass in Daltons (u) */
    real_t get_mass(const string_t &symbol) {
             if(symbol == "H" ) return 1.007825;
        else if(symbol == "C" ) return 12.0; 
        else if(symbol == "Ar") return 39.948; 
        else {
            std::cout << "Warning: Unknown atomic symbol " << symbol << "\n";
            return 1.0;
        }
    }

}

