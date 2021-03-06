/*
 * forces.hpp
 */
#ifndef FORCES_HPP
#define FORCES_HPP

#include "types.hpp"
using namespace types;
#include "decl.hpp"


namespace force {
        enum type { LennardJones = 1 };
}

class Force {
    public:
        virtual void add(State *s) =0;
        virtual ~Force() =0;
};

class LennardJones : public Force {
    private:
        real_t sigma;
        real_t epsilon;
        real_t rcut;
        real_t ecut;       /** Energy at r=rcut */
        force::type type;

    public:
        LennardJones(real_t sigma, real_t epsilon, real_t rcut):
            sigma(sigma), epsilon(epsilon), rcut(rcut), type(force::LennardJones) 
          { ecut = 4.0*epsilon*(pow(sigma/rcut, 12.0)-pow(sigma/rcut, 6.0)); }

        ~LennardJones() {}
        void add(State *s);
};

#endif
