/**
 * Demonstrates how to use the d_vec_t class on CUDA devices.
 */
#include <iostream>


#include "types.hpp"
#include "types.cuh"
using namespace types;

template<class T>
__global__ void increment(d_vec_t<T> d_vec){
    for(size_t i=0; i<d_vec.size(); ++i){
        d_vec(i) += 1;
    }
}

int main(){

    vec_t<real_t> v = vec_t<real_t>(10,1.0);
    std::cout << "Before increment: " << v(9) << std::endl;

    // Set up device vector using v
    d_vec_t<real_t> d_vec = d_vec_t<real_t>(&v);

    // increment vector on device
    increment<<<1,1>>>(d_vec);
    increment<<<1,1>>>(d_vec);
 
    // Old way, required members of d_vec_t to be public
    //vec_t<real_t> w(&d_vec);
    //std::cout << "After increment: " << w(0) << std::endl;

    //vec_t<real_t> x(d_vec.host_vec());
    //std::cout << "After increment: " << x(9) << std::endl;
    v.copy(d_vec.host_vec());
    std::cout << "After increment: " << v(9) << std::endl;


    return 0;
}

