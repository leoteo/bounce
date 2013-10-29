/*
 * types.cuh
 */
#ifndef TYPES_CUH
#define TYPES_CUH

#include "types.hpp"

// These macros hide cuda-macros in case of non-cuda compilation
#ifdef __CUDACC__
  
#define CUDA_H __host__
#define CUDA_D __device__
#define CUDA_HD __host_r __device__

#else

#define CUDA_H
#define CUDA_D
#define CUDA_HD
#endif 

// Error-check macro
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, types::string_t file, int line, bool abort=true)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file.c_str(), line);
      if (abort) exit(code);
   }
}



namespace types {

/**
 * The 'device vector' d_vec_t mimics vec_t on the device
 *
 * It is meant to be instantiated on the host,
 * then copied to and used on the device.
 *
 * The constructor takes a pointer to a vec_t on the host
 * and then performs memory allocation on and copies to the device.
 * A pointer to d_vec_t may then be passed to a kernel,
 * which can directly operate on d_vec_t.
 *
 * Note: For simplicity, the d_vec_t has constant size.
 */
template<class T>
class d_vec_t {
    private:
        T *v_;
        size_t *N_;
    public:
        CUDA_H d_vec_t(const vec_t<T> *v);
        CUDA_H d_vec_t() : v_(0), N_(0) {}
        CUDA_H vec_t<T> *host_vec() const;
        CUDA_D const T&  operator() (size_t i) const {return  v_[i];}
        CUDA_D      T&   operator() (size_t i)       {return v_[i];}
        CUDA_D size_t size() const {return *N_;}
};


template<class T>
CUDA_H d_vec_t<T>::d_vec_t(const vec_t<T> *vec){
    size_t N = vec->size();

    // Store size of vector
    gpuErrchk( cudaMalloc(&N_, sizeof(size_t)) );
    gpuErrchk( cudaMemcpy(N_, &N, sizeof(size_t), cudaMemcpyHostToDevice) );

    // Store data
    gpuErrchk( cudaMalloc(&v_, sizeof(T) * N) );
    gpuErrchk( cudaMemcpy(v_, &((*vec)(0)), sizeof(T) * N, cudaMemcpyHostToDevice) );

}

template<class T>
CUDA_H vec_t<T> * d_vec_t<T>::host_vec() const{
    // Find out size of d_vec
    size_t N;
    gpuErrchk( cudaMemcpy(&N, this->N_, sizeof(size_t), cudaMemcpyDeviceToHost) );

    // Copy contents of d_vec into data
    std::vector<T> *data = new std::vector<T>(N);
    gpuErrchk( cudaMemcpy( &((*data)[0]), &(this->v_[0]), sizeof(T) * N, cudaMemcpyDeviceToHost) );

    return new vec_t<T>(data);
}



/**
 * d_VecVec3d mimics VecVec3d on the device
 *
 * It is meant to be instantiated on the host,
 * then copied to and used on the device.
 *
 * The constructor takes a pointer to a VecVec3d on the host
 * and then performs memory allocation on and copies to the device.
 * A pointer to d_VecVec3d may then be passed to a kernel,
 * which can directly operate on d_VecVec3d.
 *
 * Note: For simplicity, the VecVec3d has constant size.
 */
class d_VecVec3d {
    private:
        d_vec_t<real_t> *v_;

    public:
        CUDA_H d_VecVec3d(VecVec3d *v);    
        CUDA_H VecVec3d *host_vec() const; 

        CUDA_D const real_t&  operator() (size_t n, size_t r) const {return (*v_)(n*3 + r);}
        CUDA_D       real_t&  operator() (size_t n, size_t r)       {return (*v_)(n*3 + r);}
        CUDA_D size_t size() const {return v_->size();}
        //TODO: Add these functions, if needed
        //CUDA_D inline VecVec3d& operator*= (real_t r);
        //CUDA_D inline VecVec3d& operator+= (const VecVec3d& w);
        //CUDA_D void zero() { size_t s = v.size(); for(size_t i=0; i < s; ++i) v(i) = 0.0; }
        //CUDA_D inline real_t abs() const;
};

CUDA_H d_VecVec3d::d_VecVec3d(VecVec3d *vec){
    // Allocate storage behind v_ to store a d_vec_t
    gpuErrchk( cudaMalloc(&v_, sizeof(d_vec_t<real_t>)) );

    // Create an appropriate d_vec_t and directly copy it behind v_
    // (its pointers already point to device memory)
    gpuErrchk( cudaMemcpy(v_, new d_vec_t<real_t>(vec->v()), sizeof(d_vec_t<real_t>), cudaMemcpyHostToDevice) );
}

CUDA_H VecVec3d* d_VecVec3d::host_vec() const{
    // Make space for d_vec_t on host
    d_vec_t<real_t> *d_vec = new d_vec_t<real_t>();
    // and copy d_vec_t from v_ back to host
    gpuErrchk( cudaMemcpy(d_vec, v_, sizeof(d_vec_t<real_t>), cudaMemcpyDeviceToHost) );

    // Finally, use it to create host vector
    return new VecVec3d( d_vec->host_vec());
}



}

#endif
