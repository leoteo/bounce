#include "types.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace types;
void from_file_oneshot(){
    // Careful with missing comment line
    string_t fname = "p.xyz";
    std::ifstream file;
    file.open(fname.c_str());

    if (file.is_open()) {
        string_t line;
        std::stringstream ss;


        // Read number of atoms
        ss << file.rdbuf();
        //std::cout << ss.str();
        size_t N;
        ss >> N ;

        
        char c[256];
        ss.getline(c, 256);        
        ss.getline(c, 256);        
        string_t comment(c);

        std::cout << comment;

        //string_t comment;
        //ss >> comment;

        // Read atoms
        real_t tmpx, tmpy, tmpz;
        string_t tmpsymbol;

        VecVec3d x = VecVec3d(N, 0.0);
        vec_t< string_t> symbols = vec_t< string_t >(N, "C");
        vec_t<real_t> m = vec_t< real_t >(N, 1.0);

        double tx,ty,tz;
        string_t ts;
        for(size_t i=0; i<N; ++i){

       //     if(!file.good()){
       //         std::cout << "Error: Unexpected end of " << fname << "\n";
       //         break;
       //     }

            //ss.clear();
            //ss.str(string_t());
            //ss.clear();
            //ss.str("");
            //std::getline(file,line);
            //ss << line;
            ss >> symbols(i) >> x(i,0) >> x(i,1) >> x(i,2);
            //ss >> ts >> tx >> ty >> tz;
            //std::cout << ts << "  "
            //    << "x " << tx
            //    << "y " << ty
            //    << "z " << tz
            //    << std::endl;
            //std::cout << ss.str() << "\n\n";

            // TODO: translate symbols into masses

        }
        x.print();
    }
}

void from_file(){
    // Works
    string_t fname = "p.xyz";
    std::ifstream file;
    file.open(fname.c_str());

    if (file.is_open()) {
        string_t line;
        std::stringstream ss;

        // Read number of atoms
        std::getline(file, line);
        ss << line;
        size_t N;
        ss >> N;

        // Discard comment line
        std::getline(file, line);
        ss << line;
        string_t comment;
        ss >> comment;

        // Read atoms
        real_t tmpx, tmpy, tmpz;
        string_t tmpsymbol;

        VecVec3d x = VecVec3d(N, 0.0);
        vec_t< string_t> symbols = vec_t< string_t >(N, "C");
        vec_t<real_t> m = vec_t< real_t >(N, 1.0);

        double tx,ty,tz;
        string_t ts;
        for(size_t i=0; i<N; ++i){

            if(!file.good()){
                std::cout << "Error: Unexpected end of " << fname << "\n";
                break;
            }

            //ss.clear();
            //ss.str(string_t());
            ss.clear();
            ss.str("");
            std::getline(file,line);
            ss << line;
            ss >> symbols(i) >> x(i,0) >> x(i,1) >> x(i,2);
            //ss >> ts >> tx >> ty >> tz;
            //std::cout << ts << "  "
            //    << "x " << tx
            //    << "y " << ty
            //    << "z " << tz
            //    << std::endl;
            //std::cout << ss.str() << "\n\n";

            // TODO: translate symbols into masses

        }
        x.print();
    }
}

void from_string(){
    std::string s = "C  1.034  2.34 23\n";
    std::stringstream ss;
    ss << s;

    std::string symbol;
    double x,y,z;
    ss >> symbol >> x >> y >> z;

    std::cout << symbol 
        << "x " << x
        << "y " << y
        << "z " << z
        << std::endl;
}

int main(){

    from_file_oneshot();

    return 0;
}

