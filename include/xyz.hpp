/*
 * xyz.hpp 
 */
#ifndef XYZ_HPP
#define XYZ_HPP

#include "types.hpp"
using namespace t;

class XyzFile {
    private:
        const std::stringstream & toStringStream() const;

    public:
        string_t fname;

        size_t N;                    /** number of atoms */
        string_t comment;            /** comment (2nd line) */
        vec_t< string_t > symbols;   /** atomic symbols */
        vex_t< real_t > m;          /** TODO: atomic masses derived from symbols*/
        VecVec3d x;                 /** positions */

        XyzFile() {}
        XyzFile(size_t N, const string_t& comment, const vec_t< real_t > &m, 
                const VecVec3d &x);
        
        void write(string_t &fn, std::ofstream::mode m);
        void write(string_t &fname);
        void append(string_t &fname);
}

#endif
