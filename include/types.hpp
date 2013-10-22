/*
 * types.hpp
 */
#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <list>
#include <vector>
#include <complex>
#include <iostream>

namespace types {

// Basic types
//typedef unsigned int index_t;
typedef int flag_t;
typedef double real_t;
typedef std::complex<real_t> complex_t;
typedef std::string string_t;


/**
 * At the moment, this just encapsulates a std::vector of real_t.
 *
 * The only difference is that vector elements are accessed as
 * v(i) and not v[i], which is the only possible way for VecVec3d lateron.
 *
 */
template<class T>
class vec_t {
    protected:
        std::vector<T> v;
    public:
        vec_t() { v = std::vector<T>(); }
        vec_t(size_t N) { v = std::vector<T>(N); }
        vec_t(size_t N, T x) { v = std::vector<T>(N, x); }
        const T&   operator() (size_t i) const {return v[i];}
              T&   operator() (size_t i)       {return v[i];}
        size_t size() const {return v.size();}
        void push_back(const T& val) {return v.push_back(val);}
};


/**
 * Encapsulates a 3d quantity per particle.
 */
class VecVec3d {
    protected:
        vec_t<real_t> v;

    public:
        VecVec3d() {}
        VecVec3d(size_t N) { v = vec_t<real_t>(3 * N, 0.0); }
        VecVec3d(size_t N, real_t r) { v = vec_t<real_t>(3 * N, r); }
        const real_t&  operator() (size_t n, size_t r) const {return v(n*3 + r);}
              real_t&  operator() (size_t n, size_t r)       {return v(n*3 + r);}
        inline VecVec3d& operator*= (real_t r);
        inline VecVec3d& operator+= (const VecVec3d& w);
        void zero() { size_t s = v.size(); for(size_t i=0; i < s; ++i) v(i) = 0.0; }      
        size_t size() const {return v.size();}
        inline void print() const;
        inline real_t abs() const;
};


inline void VecVec3d::print() const {
    size_t s = v.size() / 3; 
    for(size_t i=0; i < s; ++i)
           std::cout << "i = " << i  << ": (x,y,z) = (" 
               << v(i*3+0) << "," 
               << v(i*3+1) << "," 
               << v(i*3+2) << ")" 
               << std::endl;
}

inline VecVec3d& VecVec3d::operator*= (real_t r) {
    size_t s = v.size(); 
    for(size_t i=0; i < s; ++i) v(i)*= r; 
    return *this; 
}

inline VecVec3d& VecVec3d::operator+= (const VecVec3d& w) {
    size_t sv = this->size(), sw = w.size();
    if(sv == sw) {
        for(size_t i=0; i<sv; ++i)
            v(i) += w.v(i);
    }
    else {
        std::cout << "Error: Trying to add VecVec3d objects of different length\n";
    }

    return *this;
}


inline VecVec3d cross(const VecVec3d& v, size_t i, 
                      const VecVec3d& w, size_t j) {
    VecVec3d u = VecVec3d(1,0.0);
    u(0,0) = v(i,1) * w(j,2) - v(i,2) * w(j,1);
    u(0,1) = v(i,2) * w(j,0) - v(i,0) * w(j,2);
    u(0,2) = v(i,0) * w(j,1) - v(i,1) * w(j,0);
    return u;
}

inline VecVec3d  operator*(real_t a, const VecVec3d &v) {
    VecVec3d w = v;
    return w *= a;
}

inline real_t VecVec3d::abs() const {
    size_t s = v.size();
    real_t sum = 0;
    for(size_t i=0; i<s; ++i) sum += v(i) * v(i);
    return sqrt(sum);
}

inline real_t abs(const VecVec3d &v){
    return v.abs();
}

typedef std::list<size_t> list_t;
typedef list_t::iterator list_it_t;


}

#endif
