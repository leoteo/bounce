/*
 * state.cpp
 */

#include "state.hpp"
#include "constants.hpp"
#include "random.hpp"

State::State(
        size_t N, 
        const VecVec3d &x, 
        const vec_t<real_t> &m, 
        const vec_t<string_t> &symbols,
        vec_t<Force*> forces,
        vec_t<real_t> cell, 
        real_t t0) :
    N(N), 
    x_(new VecVec3d( x )),
    v_(new VecVec3d(N,0.0)),
    f_(new VecVec3d(N, 0.0)),
    m_(new vec_t<real_t>( m )),
    symbols_(new vec_t<string_t>( symbols )),
    forces(forces), // TODO: properly copy forces
    ePot(0.0),
    eKin(0.0),
    cell(cell) {

    /* initialize forces */
    size_t nf = forces.size();
    for(size_t i=0; i < nf; ++i){
          forces(i)->add(this);
    }

    /* initialize velocities */
    draw_v(t0);

}

//Following the rule of three...
State::State(const State &s) : 
    N(s.N),
    x_(new VecVec3d(*(s.x_)) ),
    v_(new VecVec3d(*(s.v_)) ),
    f_(new VecVec3d(*(s.f_)) ),
    m_(new vec_t<real_t>(*(s.m_)) ),
    symbols_(new vec_t<string_t>(*(s.symbols_))),
    ePot(s.ePot),
    eKin(s.eKin),
    temp(s.temp),
    cell(s.cell)    {
    // Note: overloading operator() means we cannot use initialization syntax!
    forces = s.forces;  //TODO: properly copy forces
}

void State::swap(State &s){
    std::swap(N, s.N);
    std::swap(x_, s.x_);
    std::swap(v_, s.v_);
    std::swap(f_, s.f_);
    std::swap(forces, s.forces);
    std::swap(m_, s.m_);
    std::swap(symbols_, s.symbols_);
    std::swap(ePot, s.ePot);
    std::swap(eKin, s.eKin);
    std::swap(temp, s.temp);
    std::swap(cell, s.cell);
}
    
State & State::operator=(State s) {
    swap(s);
    return *this;
}

State::~State(){
    delete x_;
    delete v_;
    delete m_;
    delete symbols_;
    for(size_t i; i<forces.size(); ++i) delete forces(i);
}


inline void State::draw_v(real_t t0) {
    NormalDistribution d = NormalDistribution();

    // Draw velocities from normal distribution of appropiate sigma
    real_t scalef;
    for(size_t i=0; i < N; ++i){
        scalef = sqrt(constants::kB * t0 / (m(i) * constants::mvsq));
        for(size_t r=0; r<3; ++r){
            v(i,r) = d.draw() * scalef;
            eKin += 0.5*m(i)*v(i,r)*v(i,r) * constants::mvsq;
        }
    }

    // Impose initial temperature t0 exactly
    real_t t = 2.0 * eKin / ((3.0 * N - 3.0) * constants::kB);
    scalef = sqrt(t0/t);

    eKin *= scalef*scalef;
    for(size_t i=0; i < N; ++i)
        for(size_t r=0; r<3; ++r)
            v(i,r) *= scalef;

}

VecVec3d State::pTot() const {
    VecVec3d pTot = VecVec3d(1, 0.0);
    for(size_t i=0; i < N; ++i){
        for(size_t r=0; r<3; ++r){
            pTot(0,r) += m(i) * v(i,r);
        }
    }
    return pTot;
}

VecVec3d State::lTot() const {
    VecVec3d lTot = VecVec3d(1,0.0);
    for(size_t i=0; i < N; ++i){
        lTot += m(i) * cross(*x_, i, *v_, i);
    }
    return lTot;
}

