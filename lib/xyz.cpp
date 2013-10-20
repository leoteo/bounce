/*
 * xyz.cpp
 */
#include "xyz.hpp"
#include "types.hpp"
#include "constants.hpp"
#include <fstream>
#include <iostream>
using namespace types;


void XyzFile::read(const string_t &fn){
    fname = fn;

    std::ifstream file;
    file.open(fname.c_str());

    if (file.is_open()) {
        // Populate stringstream
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        // Read number of atoms
        ss >> N;

        // Reading the comment line is a bit problematic
        // since stringstream skips whitespace
        char c[256];
        ss.getline(c,256); // should get the rest of the 1st line
        ss.getline(c,256); // should get the 2nd line
        comment = string_t(c);

        // Read atoms
        x = VecVec3d(N, 0.0);
        symbols = vec_t< string_t >(N, "C");
        m = vec_t< real_t >(N, 1.0);
        for(size_t i=0; i<N; ++i){

            if(!ss.good()){
                std::cout << "Error: Could not parse " << fname 
                          << " at line "<< i+3 << "\n";
                break;
            }

            ss >> symbols(i) >> x(i,0) >> x(i,1) >> x(i,2);
            // TODO: translate symbols into masses

        }
    }

}

string_t XyzFile::to_string() const {
    std::stringstream ss;

    ss << "   " << N << std::endl;
    ss << comment << std::endl;

    for(size_t i=0; i<N; ++i){
        ss << symbols(i) << " "
           << std::scientific << x(i,0) << " "
           << std::scientific << x(i,1) << " "
           << std::scientific << x(i,2)
           << std::endl;
    }

    return ss.str();
}

void XyzFile::write(const string_t &fn, std::ios_base::openmode m) {
    fname = fn;

    std::ofstream of;
    of.open(fname.c_str(), m);

    of << to_string();

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
