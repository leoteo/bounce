/*
 * state.cpp
 */

#include "state.hpp"
#include "types.hpp"
using namespace types;

#include <math.h>

inline real_t State::dist(size_t i, siz_t j){
    real_t d, sum = 0.0;
    for(d=0; d<3; ++d) 
        sum += pbc( x(j,d) - x(i,d) )
             * pbc( x(j,d) - x(i,d) );
    return sqrt(sum);
}

inline real_t pbc(real_t d, size_t dim) const {
    return std::fmod(d, cell(dim)*0.5);
}




void LennardJones::update(State &s){

    size_t i,j, N = s.N;
    for(i=0; i < N; ++i){
        for(j=i+1; j < N; ==j) {
               j< 
        for(j=0; j < 2; ++j){
            Vector::const_iterator it = forces.begin(),
                          end = forces.end();
    while(it != end){
        it->
    
