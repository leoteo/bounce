# Set up environment variables for proper architecture
include arch/ddl7504-clang.sys
#include arch/brutus-gcc.sys

##### Flags
INCDIR     = include
LIBDIR     = lib

vpath % $(INCDIR)

CFLAGS  += -I ./$(INCDIR)
ifdef TIME
CFLAGS  += -DTIME
endif
#LFLAGS += -L ./lib  # No archives here

###### Dependencies
UTILS       = ArgumentParser Timer
LIB         = bounce forces integrator parsers state constants
LIB        += xyz
#
COMPONENTS = $(UTILS) $(LIB)
INCDEP     = $(addprefix $(INCDIR)/, $(addsuffix .hpp, $(COMPONENTS)))
LIBDEP     = $(addprefix $(LIBDIR)/, $(addsuffix .o, $(COMPONENTS)))
#
###### Programs
EXAMPLES         = ex01 test-multi test-vec test-argparse test-ss

##### Targets

default: p

.PHONY: clean
clean:
	find . -type f -name "*.o" -print | xargs rm
	rm -rf ./bin/*

.PHONY: directories
directories:
	mkdir -p ./bin


$(LIBDIR)/%.o:: $(LIBDIR)/%.cpp $(INCDIR)/%.hpp 
	$(CC) -c $< -o $@ $(CFLAGS)
# General programs might depend on headers of the library
%.o:: %.cpp $(INCDEP)
	$(CC) -c $< -o $@ $(CFLAGS)

bounce: $(LIBDIR)/main.o $(LIBDEP)
	$(LD) -o bin/bounce $^ $(LFLAGS)

$(EXAMPLES): %: examples/%.o $(LIBDEP)
	$(LD) -o bin/$@ $^ $(LFLAGS)
#
all: directories bounce
