/*
 * xyz.cpp
 */
#include "xyz.hpp"
#include "types.hpp"
#include <fstream>
#include <iostream>
using namespace types;


void XyzFile::read(const string_t &fn){
    fname = fn;

    std::ifstream file;
    file.open(fname.c_str());

    if (file.is_open()) {
        string_t line;
        std::stringstream ss;

        // Read number of atoms
        std::getline(file, line);
        ss << line;
        ss >> N;

        // Discard comment line
        std::getline(file, line);
        ss << line;
        ss >> comment;

        // Read atoms
        real_t tmpx, tmpy, tmpz;
        string_t tmpsymbol;

        x = VecVec3d(N, 0.0);
        symbols = vec_t< string_t >(N, "C");
        m = vec_t< real_t >(N, 1.0);
        for(size_t i=0; i<N; ++i){

            if(!file.good()){
                std::cout << "Error: Unexpected end of " << fname << "\n";
                break;
            }

            std::getline(file,line);
            ss << line;
            ss >> symbols(i) >> x(i,0) >> x(i,1) >> x(i,2);
            // TODO: translate symbols into masses

        }
    }

}

string_t XyzFile::to_stringstream() const {
    std::stringstream ss;

    ss << "   " << N << std::endl;
    ss << comment;

    for(size_t i=0; i<N; ++i){
        ss << symbols(i) 
           << std::scientific << x(i,0)
           << std::scientific << x(i,1)
           << std::scientific << x(i,2)
           << std::endl;
    }

    return ss.str();
}

void XyzFile::write(const string_t &fn, std::ios_base::openmode m) {
    fname = fn;

    std::ofstream of;
    of.open(fname.c_str(), m);

    of << to_stringstream();

    of.close();

}

void XyzFile::write(const string_t &fn) {
    write(fn, std::ofstream::trunc);
}

void XyzFile::append(const string_t &fn) {
    write(fn, std::ofstream::app);
}

XyzFile::XyzFile(size_t N, const string_t& comment, const vec_t<real_t> &m,
                const VecVec3d &x) :  N(N), comment(comment), m(m), x(x) {
    //TODO: translate masses to symbols
    symbols = vec_t < string_t > (N, "C");
}
