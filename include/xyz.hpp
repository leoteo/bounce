/*
 * xyz.hpp 
 */
#ifndef XYZ_HPP
#define XYZ_HPP

#include "types.hpp"
using namespace types;

class XyzFile {
    private:
        string_t to_string() const;
        string_t fname;

    public:

        size_t N;                    /** number of atoms */
        string_t comment;            /** comment (2nd line) */
        vec_t< string_t > symbols;   /** atomic symbols */
        vec_t< real_t > m;          /** TODO: atomic masses derived from symbols*/
        VecVec3d x;                 /** positions */

        XyzFile() {}
        XyzFile(size_t N, const string_t& comment, const vec_t< real_t > &m, 
                const VecVec3d &x);
        
        void read(const string_t &fn);
        void write(const string_t &fn, std::ios_base::openmode m);
        void write(const string_t &fn);
        void append(const string_t &fn);
};

#endif
