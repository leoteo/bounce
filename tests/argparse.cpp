#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/format.hpp>
namespace po = boost::program_options;

#include <ctime>
time_t t = clock();

#include "types.hpp"
using namespace types;

/***********************************************
  Declarations
***********************************************/

// Returns true, if command line arguments parse ok
bool parse(int ac, char* av[], po::variables_map& vm);


/***********************************************
  Implementations
***********************************************/

int main(int ac, char* av[]) {

    po::variables_map args;
    if (parse(ac, av, args)) {
        std::vector<real_t> v;
    
 //       // Iterate over cubes
 //       if(args.count("isovalues")){
 //               v = args["isovalues"].as< std::vector<Real> >();
 //               m = stm::StmCube::CONSTANT_CURRENT;
 //        }
 //        else{
 //               v = args["zvalues"].as< std::vector<Real> >();
 //               m = stm::StmCube::CONSTANT_HEIGHT;
 //        }
    }

    return 0;
}




bool parse(int ac, char* av[], po::variables_map& vm) {

    std::string input_file;
    // Declare regular options
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version information")
    ("input-file,i", po::value<string_t>(&input_file), "Input file specifying all or several of the following options")
    ("dt",po::value< real_t >()->default_value(0.5), "Timestep for integration.")
    ("t", po::value< real_t >()->default_value(100), "Total integration time.")
    ("fil_m", po::value<string_t>()->default_value("m"), "File with list of masses")
    ("fil_v", po::value<string_t>()->default_value("v"), "File with list of velocities")
    ("fil_x", po::value<string_t>()->default_value("x"), "File with list of positions")
    // for simplicity, we allow just one kind of interaction for the moment
    ("f", po::value< string_t >()->default_value("lj"), "Interactions between particles. Can be 'lj' (Lennard-Jones) or 'g' (Gravity).")
    ("lj_sigma",po::value< real_t >()->default_value(1.0), "Lennard-Jones sigma")
    ("lj_epsilon",po::value< real_t >()->default_value(1.0), "Lennard-Jones epsilon")
    ("lj_rcut",po::value< real_t >()->default_value(1.0), "Lennard-Jones cutoff radius")
    ;

    // Register positional options
    po::positional_options_description p;
    p	.add("input-file", 1) ;

    // Parse
    po::store(po::command_line_parser(ac,av).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    // If specified, try to parse conig file
    if (vm.count("input-file")){
        std::ifstream ifs(input_file.c_str());
       if(!ifs) std::cout << "Error: cannot access " << input_file << "\n";
       store(parse_config_file(ifs, desc), vm);
       notify(vm);
    }

    // Check if all necessary things are specified
    if (	vm.count("help") || false
       ){
        std::cout << "Usage: bounce [options]\n";
        std::cout << desc << "\n";
    } else if (vm.count("version")) {
        std::cout << "Oct 16th 2013\n";
    } else {
        return true;
    }

    return false;
}
