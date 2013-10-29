/**
 * Demonstrates how to use the d_VecVec3d class on CUDA devices.
 */ 

#include <iostream>
#include <vector>
#include <string>


#include "types.hpp"
#include "types.cuh"
using namespace types;


__global__ void increment(d_VecVec3d d_vec){
    for(size_t i=0; i<d_vec.size(); ++i){
        d_vec(i,0) += 1;
    }
}

int main(){

    VecVec3d v = VecVec3d(10,1.0);
    std::cout << "Before increment: " << v(0,0) << std::endl;

    // Set up device vector using v
    d_VecVec3d d_vec = d_VecVec3d(&v);

    // increment vector on device
    increment<<<1,1>>>(d_vec);

    // copy back to host
    v.copy(d_vec.host_vec());
    std::cout << "After increment: " << v(0,0) << std::endl;

    return 0;
}

