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
 * vec_t basically encapsulates a std::vector
 *
 * The only difference is that vector elements are accessed as
 * v(i) and not v[i], which is the only possible way for VecVec3d lateron.
 * Furthermore, we store a *pointer* to std::vec, which has the advantage
 * that vec_t can operate on a preexisting std::vector/vec_t
 *  without the need to copy the data.
 *
 */
template<class T>
class vec_t {
    protected:
        std::vector<T> *v_;
    public:
        vec_t() { v_ = new std::vector<T>(); }
        vec_t(size_t N) { v_ = new std::vector<T>(N); }
        vec_t(size_t N, T x) { v_ = new std::vector<T>(N, x); }
        vec_t(std::vector<T> *v) { v_ = v; }
        vec_t(vec_t<T> *vec) { v_ = vec->v_; }

        // Following the rule of three...
        vec_t(const vec_t<T> &w) { v_ = new std::vector<T>( *(w.v_) ); }
        void swap(vec_t<T> &w) { std::swap(v_, w.v_); }
        inline vec_t<T> & operator=(vec_t<T> w);
        ~vec_t() { delete v_; }

        const T&   operator() (size_t i) const {return v_->at(i);}
             T&   operator() (size_t i)       {return v_->at(i);}
        size_t size() const {return v_->size();}
        void push_back(const T& val) {return v_->push_back(val);}
        void copy(vec_t<T> *vec) { v_ = vec->v_; }
};

template<class T>
inline vec_t<T> & vec_t<T>::operator=(vec_t<T> w) {
    swap(w); 
    return *this;
} 
/**
 * Encapsulates a 3d quantity per particle.
 */
class VecVec3d {
    protected:
        vec_t<real_t> *v_;

    public:
        VecVec3d() { v_ = new vec_t<real_t>(); }
        VecVec3d(size_t N) { v_ = new vec_t<real_t>(3 * N, 0.0); }
        VecVec3d(size_t N, real_t r) { v_ = new vec_t<real_t>(3 * N, r); }
        VecVec3d(vec_t<real_t> *v) {v_ = v;}
        VecVec3d(VecVec3d *v) {v_ = v->v_;}
        vec_t<real_t> * v() const { return v_; }  

        // Following the rule of three...
        VecVec3d(const VecVec3d &w) { v_ = new vec_t<real_t>( *(w.v_) ); }
        void swap(VecVec3d &w) { std::swap(v_, w.v_); }
        VecVec3d &operator=(VecVec3d w);
        ~VecVec3d(){ delete v_; }


        const real_t&  operator() (size_t n, size_t r) const {return (*v_)(n*3 + r);}
              real_t&  operator() (size_t n, size_t r)       {return (*v_)(n*3 + r);}
        inline VecVec3d& operator*= (real_t r);
        inline VecVec3d& operator+= (const VecVec3d& w);
        void zero() { size_t s = v_->size(); for(size_t i=0; i < s; ++i) (*v_)(i) = 0.0; }      
        size_t size() const {return v_->size();}
        inline void print() const;
        inline real_t abs() const;
        void copy(VecVec3d *v) { v_ = v->v_; }
};

inline VecVec3d & VecVec3d::operator=(VecVec3d w) {
    swap(w); 
    return *this;
} 

inline void VecVec3d::print() const {
    size_t s = v_->size() / 3; 
    for(size_t i=0; i < s; ++i)
           std::cout << "i = " << i  << ": (x,y,z) = (" 
               << (*v_)(i*3+0) << "," 
               << (*v_)(i*3+1) << "," 
               << (*v_)(i*3+2) << ")" 
               << std::endl;
}

inline VecVec3d& VecVec3d::operator*= (real_t r) {
    size_t s = v_->size(); 
    for(size_t i=0; i < s; ++i) (*v_)(i)*= r; 
    return *this; 
}

inline VecVec3d& VecVec3d::operator+= (const VecVec3d& w) {
    size_t sv = this->size(), sw = w.size();
    if(sv == sw) {
        for(size_t i=0; i<sv; ++i)
            (*v_)(i) += (*w.v_)(i);
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
    size_t s = v_->size();
    real_t sum = 0;
    for(size_t i=0; i<s; ++i) sum += (*v_)(i) * (*v_)(i);
    return sqrt(sum);
}

inline real_t abs(const VecVec3d &v){
    return v.abs();
}




//typedef std::list<size_t> list_t;
//typedef list_t::iterator list_it_t;


}

#endif
