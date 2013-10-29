/*
 * forces.cpp
 */

#include "forces.hpp"
#include "state.hpp"
#include "types.hpp"
using namespace types;

Force::~Force() {}


void LennardJones::add(State *s){

    size_t i,j,d, N = s->N;

    real_t r, rx, ry, rz, f;
    real_t pow6, pow12;
    for(i=0; i < N; ++i){
        for(j=i+1; j < N; ++j) {
            r = s->dist(i,j);
            if (r < rcut){
                if (r * 3 < sigma)
                    std::cout << "Warning: atoms " << i+1 
                              << " and " << j+1 << " too close.\n";

                pow6 = pow(sigma/r, 6.0);
                pow12 = pow(sigma/r, 12.0);

                // Update potential energy
                s->ePot += 4.0*epsilon*(pow12 - pow6) - ecut;

                // Update forces
                f = -4.0*epsilon*(-12.0*pow12 +6.0*pow6 ) /r;
                for(d=0; d<3; ++d){
                    s->f(i,d) += s->cell.pbc(s->x(i,d) - s->x(j,d), d)/r * f;
                    s->f(j,d) -= s->cell.pbc(s->x(i,d) - s->x(j,d), d)/r * f;
                }
            }
        }
    }
}



//__global__ void() add_(State *s, Force f)
//    //const thrust::device_vector<float> &d_x, thrust::device_vector<float> &d_f){
//
//    size_t i = threadIdx.x;
//
//
//    for(size_t j=i+1; j < N; ++j) {
//        // Problem here: would need to copy whole state object
//        // => need separate dist function that operatres on d_x
//        // and knows the pbc!
//        r = s->dist(i,j);
//        if (r < rcut){
//            // need to know lennard-jones parameters as well
//            if (r * 3 < sigma)
//                std::cout << "Warning: atoms " << i+1 
//                          << " and " << j+1 << " too close.\n";
//
//            pow6 = pow(sigma/r, 6.0);
//            pow12 = pow(sigma/r, 12.0);
//
//            // Update potential energy
//            // Should make this act on local copy of ePot
//            s->ePot += 4.0*epsilon*(pow12 - pow6) - ecut;
//
//            // Update forces
//            f = -4.0*epsilon*(-12.0*pow12 +6.0*pow6 ) /r;
//            for(d=0; d<3; ++d){
//                s->f(i,d) += s->pbc(s->x(i,d) - s->x(j,d), d)/r * f;
//                // This update may result in race conditions...
//                // Would it be more clever to do twice the computation?
//                // -  Or should each thread have a local copy of the force vector?
//                //    I guess not.
//                s->f(j,d) -= s->pbc(s->x(i,d) - s->x(j,d), d)/r * f;
//            }
//        }
//    }
//}
//
//
//void LennardJones::add(State *s){
//
////    size_t i,j,d, N = s->N;
////
////    real_t r, rx, ry, rz, f;
////    real_t pow6, pow12;
////
////
//
//    // Allocate state on device
//    State *d_s;
//    gpuErrchk( cudaMalloc(&d_s, sizeof(State)) );
//    gpuErrchk( cudaMemcpy(d_s, s, sizeof(State), cudaMemcpyHostToDevice) );
//
//    // Copy positions as well
//    vec_t<
//
//
//    // Copy positions to device
//    thrust::device_vector<float> d_x = s->x.v;
//    // Copy forces to device
//    thrust::device_vector<float> d_f = s->f.v;
//
//    size_t N = s->N;
//    add_<<<1,s->N>>>add_(d_x, d_f);
//}
//
//    
