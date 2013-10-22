/*
 * parsers.cpp
 */
#include <string>
#include <fstream>
#include <iomanip>
#include "parsers.hpp"
#include "integrator.hpp"
#include "state.hpp"
#include "xyz.hpp"
#include "types.hpp"
using namespace types;
#include "constants.hpp"
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
    ("dt",po::value< real_t >()->default_value(0.5), "Timestep for integration [fs]")
    ("t", po::value< real_t >()->default_value(100), "Total integration time [fs]")
    ("nwrite", po::value< size_t >()->default_value(100), "Write info each x steps.")
    ("int", po::value< string_t >()->default_value("vv"), "Integrator. Can be 'vv' (Velocity Verlet).")
//    ("fil_m", po::value<string_t>()->default_value("m"), "File with list of masses")
//    ("fil_v", po::value<string_t>()->default_value("v"), "File with list of velocities")
//    ("fil_x", po::value<string_t>()->default_value("x"), "File with list of positions")
    ("fil_xyz", po::value<string_t>(), ".xyz file containing positions and atomic symbols. See http://openbabel.org/wiki/XYZ_%28format%29")
    ("temp0",po::value< real_t >()->default_value(300), "Initial temperature [K]")
    // for simplicity, we allow just one kind of interaction for the moment
    ("f", po::value< string_t >()->default_value("lj"), "Interactions between particles. Can be 'lj' (Lennard-Jones) or 'g' (Gravity).")
    ("lj_sigma",po::value< real_t >()->default_value(3.4), "Lennard-Jones sigma [Angstroms]")
    ("lj_epsilon",po::value< real_t >()->default_value(0.997), "Lennard-Jones epsilon [kJ/mol]")
    ("lj_rcut",po::value< real_t >()->default_value(8.5), "Lennard-Jones cutoff radius [Angstroms]")
    ("cell_x",po::value< real_t >()->default_value(10.0), "Cell size along x [Angstroms]")
    ("cell_y",po::value< real_t >()->default_value(10.0), "Cell size along y [Angstroms]")
    ("cell_z",po::value< real_t >()->default_value(10.0), "Cell size along z [Angstroms]")
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
       if(!ifs)  std::cout << "Error: cannot access " << input_file << "\n";
       store(parse_config_file(ifs, desc), vm);
       notify(vm);
    }

    // Check if all necessary things are specified
    if (	vm.count("help") || false
       ){
        std::cout << "Usage: bounce [options]\n";
        std::cout << desc << "\n";
        done = true;
    } else if (vm.count("version")) {
        std::cout << "Oct 16th 2013\n";
        done = true;
    } else {
        done = false;
    }

}

Integrator *InputParser::get_integrator() const {
    // Setting up the integrator
    if(vm["int"].as< string_t >() == "vv"){
        return new VelocityVerlet(
               vm["dt"].as< real_t >(), 
               size_t(vm["t"].as< real_t >() / vm["dt"].as< real_t>())
            );
    } else {
        std::cout << "Error: invalid integrator "<< vm["int"].as< string_t >() 
                  << ".\n";
        return 0;
    }
}

State *InputParser::get_state() const {
    // Setting up the state

    // parameters of simulation cell
    vec_t < real_t > cell;
    cell.push_back(vm["cell_x"].as< real_t >());
    cell.push_back(vm["cell_y"].as< real_t >());
    cell.push_back(vm["cell_z"].as< real_t >());

    // Forces
    vec_t< Force* > forces;
    if(vm["f"].as< string_t >() == "lj"){

        // Check that cell is large enough
        real_t rcut = vm["lj_rcut"].as< real_t >();
        for(size_t i=0; i<3; ++i){
            if(cell(i) < 2*rcut)
                std::cout << "Warning: Cell dimension "<< i+1 << " < 2*rcut in violation of the minimum image convention.\n";
        }

        forces.push_back(
            new LennardJones(
                vm["lj_sigma"].as< real_t >(),
                vm["lj_epsilon"].as< real_t >(),
                vm["lj_rcut"].as< real_t >()
            )
        );
    } else {
        std::cout << "Error: invalid force "<< vm["f"].as< string_t >() 
                  << ".\n";
        return 0;
    }


    if( vm.count("fil_xyz") ){
        XyzFile xyz = XyzFile();
        xyz.read(vm["fil_xyz"].as< string_t >());

        return new State(xyz.N, xyz.x, xyz.m, xyz.symbols, forces, 
                         cell, vm["temp0"].as< real_t >());
    }
    else {
        // At the moment we don't support the fil_m fil_x fil_v
        std::cout << "Error: No initial positions provided.\n";
        return 0;
    }
}


OutputParser *InputParser::get_outputparser() const {
    // Setting up the OutputParser
    return new OutputParser( vm["nwrite"].as< size_t >() );

}

void OutputParser::write(const Integrator *i, const State *s){
    if (i->get_step() % nwrite == 0){
        std::stringstream ss;
        //ss.setf( std::ios_base::fixed, std::ios_base::floatfield );
        ss << "step = " << i->get_step() << ", "
           << "t = " << i->t() << " fs, "
            << std::fixed << std::setprecision(6)
           << "eKin = "  << s->eKin << " kJ/mol, "
           << "ePot = " << s->ePot << " kJ/mol, "
           << "eTot = " << s->eTot() << " kJ/mol, "
            << std::scientific << std::setprecision(8)
           << "|pTot| = "  << abs(s->pTot()) << " u A/fs, "
           << "|lTot| = " << abs(s->lTot()) << " u A/fs\n";

        std::cout << ss.str();

        XyzFile xyz(s->N, "", s->symbols, s->x);
        xyz.append("traj.xyz");
    }
}
