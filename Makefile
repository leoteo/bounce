# Set up environment variables for proper architecture
include arch/ddl7504-clang.sys

##### Flags
INCDIR     = include
LIBDIR     = lib

vpath % $(INCDIR)

CFLAGS  += -I ./$(INCDIR)
#LFLAGS += -L ./lib  # No archives here

###### Dependencies
UTILS       = ArgumentParser Timer
#
COMPONENTS = $(UTILS)
INCDEP     = $(addprefix $(INCDIR)/, $(addsuffix .hpp, $(COMPONENTS)))
LIBDEP     = $(addprefix $(LIBDIR)/, $(addsuffix .o, $(COMPONENTS)))
#
###### Programs
EXAMPLES         = ex01

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

$(EXAMPLES): %: examples/%.o $(LIBDEP)
	$(LD) -o bin/$@ $^ $(LFLAGS)
#
all: directories $(STMPROGS) $(UTILPROGS)
