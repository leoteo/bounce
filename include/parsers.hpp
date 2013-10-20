/*
 * parsers.hpp
 */
#ifndef PARSERS_HPP
#define PARSERS_HPP

#include "types.hpp"
using namespace types;
#include "decl.hpp"
#include <boost/program_options.hpp>

namespace parsers {
    enum outtype { Console = 1 };
}

class OutputParser {
    private:
        size_t nwrite;
    public:
        OutputParser() {}
        void write(const Integrator *i, const State *s);
        OutputParser(size_t nwrite) : nwrite(nwrite) {}
};

class InputParser {
    private:
        int ac;
        char** av;
        boost::program_options::variables_map vm;
        bool done;
    public:
        InputParser() {}
        InputParser(int ac, char* av[]);

        Integrator *get_integrator() const;
        State *get_state() const;
        OutputParser *get_outputparser() const;
        bool is_done() { return done; }
};


#endif
