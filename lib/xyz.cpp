/*
 * xyz.cpp
 */
#include "xyz.hpp"
#include "types.hpp"
using namespace types;


bool XyzFile::read(string_t &fn){
    fname = fn;

    std::ifstream file;
    file.open(fname.c_str());

    if (file.is_open()) {
        String line;
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
                return false;
            }

            std::getline(file,line);
            ss << line;
            ss >> symbols(i) >> x(i,0)x >> x(i,1) >> x(i,2);
            // TODO: translate symbols into masses

        }

        return true;
}

const std::stringstream & toStringStream(){
    std::stringstream ss;

    ss << "   " << N << std::endl;
    ss << comment;

    for(size_t i=0; i<N; ++i){
        ss << symbol(i) 
           << std::scientific << x(i,0)
           << std::scientific << x(i,1)
           << std::scientific << x(i,2)
           << std::endl;
    }

    return ss;
}


void XyzFile::write(string_t &fn, std::ofstream::mode m){
    fname = fn;

    std::ofstream of;
    of.open(fname.c_str(), m);

    of << toStringStream().rdbuf();

    of.close();

}

void XyzFile::write(string_t &fn){
    write(fn, std::ofstream::trunc);
}

void XyzFile::append(string_t &fn){
    write(fn, std::ofstream::app);
}

XyzFile::XyzFile(size_t N, const string_t& comment, const vec_t<real_t> &m,
                const VecVec3d &x) :  N(N), comment(comment), m(m), x(x) {
    //TODO: translate masses to symbols
    symbols = vec_t < string_t > (N, "C");
}
