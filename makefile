################################################################################
# @file makefile
#
# @brief makefile for calc (calculator) 
################################################################################

################################################################################
# Build a debug (DEBUG=1: default) or release (DEBUG=0) binary?
################################################################################

CXXFLAGS+=-std=c++11 -Wall -Wextra

DEBUG	?= 1
ifeq	($(DEBUG),1)
	CXXFLAGS += -Og -g3 -DDEBUG
else
	CXXFLAGS += -O3 -DNDEBUG
endif

################################################################################
# Project files
################################################################################

SRCS	= calc.cpp math.cpp parser.cpp symbol.cpp token.cpp utils.cpp
OBJS	= $(SRCS:.cpp=.o)
EXE		= calc

################################################################################
#	The default target...
################################################################################

.PHONY:	all clean cleanall docs help pr test

all:	$(EXE) docs

################################################################################
# $(EXE) (calc)
################################################################################

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

################################################################################
# Header dependencies
################################################################################

calc.o:		utils.h math.h parser.h symbol.h token.h
math.o:		math.h utils.h
parser.o:	math.h parser.h symbol.h utils.h
symbol.o: 	symbol.h
token.o:	symbol.h token.h utils.h
utils.o:	utils.h

################################################################################
# Cleanup intermediates...
################################################################################

clean:
	@rm -f $(OBJS)

################################################################################
# Cleanup all targets and intermediates...
################################################################################

cleanall: clean
	@rm -rf $(EXE) docs

################################################################################
# Generate documentation
################################################################################

docs:
	@doxygen

################################################################################
# Print a help message...
################################################################################

help:
	@echo "Usage:  make [DEBUG=0|1] targets..."
	@echo ""
	@echo "DEBUG=1, (default), builds a debug image and 0 builds a release."
	@echo ""
	@echo "Targets:"
	@echo "    all     - to build calc and generate documentation (default)."
	@echo "    calc    - to build the calculator."
	@echo "    clean   - to delete intermediates."
	@echo "    cleanll - to delete all targets and intermediates."
	@echo "    docs    - to generate documentation."
	@echo "    help    - prints this message."
	@echo "    pr      - print source"
	@echo "    test    - to bring calc upto date and run tests."
	@echo ""

################################################################################
# Print
################################################################################

pr:
	@pr $(SRCS) makefile | expand -4 | lpr

################################################################################
# Bring calc up to date and run some tests...
################################################################################

test: all
	./xcalc.sh

