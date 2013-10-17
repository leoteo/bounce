/*
 * parsers.cpp
 */
#include <string>
#include "parsers.hpp"
#include "integrator.hpp"
#include "state.hpp"
#include "xyz.hpp"
#include "types.hpp"
using namespace types;
#include <boost/program_options.hpp>
namespace po = boost::program_options;



InputParser::InputParser(int ac, char* av[]) : ac(ac), av(av){

    std::string input_file;
    // Declare regular options
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version information")
    ("input-file,i", po::value<string_t>(&input_file), "Input file specifying all or several of the following options")
    ("dt",po::value< real_t >()->default_value(0.5), "Timestep for integration.")
    ("t", po::value< real_t >()->default_value(100), "Total integration time.")
    ("nwrite", po::value< size_t >()->default_value(100), "Write info each x steps.")
    ("int", po::value< string_t >()->default_value("lj"), "Integrator. Can be 'vv' (Velocity Verlet).")
//    ("fil_m", po::value<string_t>()->default_value("m"), "File with list of masses")
//    ("fil_v", po::value<string_t>()->default_value("v"), "File with list of velocities")
//    ("fil_x", po::value<string_t>()->default_value("x"), "File with list of positions")
    ("fil_xyz", po::value<string_t>(), ".xyz file containing positions and atomic symbols")
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

}


void InputParser::init(Integrator &i, State &s, OutputParser &o){

    // Setting up the integrator
    if(vm["int"].as< string_t >() == "vv"){
        i = VelocityVerlet(
               vm["dt"].as< real_t >(), 
               size_t(vm["t"].as< real_t >() / s.dt)
            );
    }

    // Setting up the state

    vec_t< Force > forces;
    if(vm["f"].as< string_t >() == "lj"){
        forces.push_back(
            LennardJones(
                vm["lj_sigma"].as< real_t >(),
                vm["lj_epsilon"].as< real_t >(),
                vm["lj_rcut"].as< real_t >()
            )
        );
    }

    if( vm.cont("fil_xyz") ){
        XyzFile xyz();
        xyz.read(vm["fil_xyz"].as< string_t >());

        // TODO: add initial temperature and draw velocities from gaussian dist.
        s = State(xyz.N, xyz.x, VecVec3d(xyz.N, 0.0), xyz.m, forces)
    }
    else {
        // At the moment we don't support the fil_m fil_x fil_v
        std::cout << "Error: No initial positions provided.\n";
    }

    // Setting up the OutputParser
    o = OutputParser( vm["nwrite"].as< size_t >() );

}

void OutputParser::write(const Integrator &i, const State &s){
    if (i.step % nwrite == 0){
        std::stringstream ss;
        ss << "step = " << i.step << ", "
           << "eKin = " << s.eKin << ", "
           << "ePot = " << s.ePot << ", "
           << "eTot = " << s.eTot << "\n";

        std::cout << ss;

        XyzFile xyz(s.N, "", s.m, s.x);
        xyz.append("POS.xyz");
    }
}
