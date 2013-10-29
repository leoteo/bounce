/*
 * state.hpp 
 */
#ifndef STATE_HPP
#define STATE_HPP

#include "types.hpp"
using namespace types;
#include "forces.hpp"
#include <math.h>

class Cell {
    private:
        vec_t< real_t > c;
    public:
        Cell() {}
        Cell(const vec_t< real_t > &c) : c(c) {}

        inline real_t pbc(real_t d, size_t dim) const;
};



inline real_t Cell::pbc(real_t d, size_t dim) const {
    while( d >  0.5 * c(dim)) d -= c(dim);
    while( d < -0.5 * c(dim)) d += c(dim);
    return d;
}

/**
 * State of a simulation
 *
 * All data structures of O(N) (or larger)
 * are pointers.
 */
class State {
    protected:
        VecVec3d *x_;
        VecVec3d *v_;
        VecVec3d *f_;
        vec_t<real_t> *m_;              /** Masses of particles */
        vec_t< string_t > *symbols_;    /** Symbols for particles */

    public:
        size_t N;         /** Number of particles */

        vec_t<Force*> forces;

        real_t ePot;      /** Potential energy */
        real_t eKin;      /** Kinetic energy */
        real_t temp;      /** Temperature */

        Cell cell;  /** only rectangular cell supported */

        State(size_t N) : 
            N(N),
            x_(new VecVec3d(N)),
            v_(new VecVec3d(N)),
            f_(new VecVec3d(N)),
            forces(vec_t<Force*>()),
            m_(new vec_t<real_t>(N, 1.0)),
            symbols_(new vec_t<string_t>(N, "")),
            ePot(0.0),
            eKin(0.0),
            temp(0.0),
            cell(Cell(vec_t<real_t>(3,1.0))) 
                {}
        State() {State(0);}
        State(size_t N,                /** to be used from xyz file */
              const VecVec3d &x, 
              const vec_t<real_t> &m, 
              const vec_t<string_t> &symbols,
              vec_t<Force*> forces, 
              vec_t<real_t> cell,
              real_t t0); 

        //Following the rule of three...
        State(const State &s);
        void swap(State &s);
        State &operator=(State s);
        ~State();

        inline void draw_v(real_t t0);

        inline real_t eTot() const { return eKin + ePot; }
        inline real_t dist(size_t i, size_t j) const;
        VecVec3d pTot() const;
        VecVec3d lTot() const;


        // These get/set functions were made necessary by CUDA
        VecVec3d& x() const { return *x_; }
        const real_t&  x(size_t n, size_t r) const {return (*x_)(n,r);}
              real_t&  x(size_t n, size_t r)       {return (*x_)(n,r);}
        VecVec3d& v() const { return *v_; }
        const real_t&  v(size_t n, size_t r) const {return (*v_)(n,r);}
              real_t&  v(size_t n, size_t r)       {return (*v_)(n,r);}
        VecVec3d& f() const { return *f_; }
        const real_t&  f(size_t n, size_t r) const {return (*f_)(n,r);}
              real_t&  f(size_t n, size_t r)       {return (*f_)(n,r);}
        vec_t<real_t>& m() const { return *m_; }
        const real_t&  m(size_t n) const {return (*m_)(n);}
              real_t&  m(size_t n)       {return (*m_)(n);}
        vec_t<string_t>& symbols() const { return *symbols_; }
        const real_t&  symbols(size_t n, size_t r) const {return (*f_)(n,r);}
              real_t&  symbols(size_t n, size_t r)       {return (*f_)(n,r);}

        
};

inline real_t State::dist(size_t i, size_t j) const {
    real_t d, sum = 0.0;
    for(d=0; d<3; ++d) 
        sum += cell.pbc( x(j,d) - x(i,d), d )
             * cell.pbc( x(j,d) - x(i,d), d );
    return sqrt(sum);
}

#endif
