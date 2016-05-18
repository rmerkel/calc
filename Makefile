################################################################################
# @file makefile
#
# @brief makefile for calc (calculator) 
################################################################################

################################################################################
# Build a debug (DEBUG=1: default) or release (DEBUG=0) binary?
################################################################################

# Support C++11, enable all, extra warnings, and generate dependency files
CXXFLAGS+=-std=c++11 -Wall -Wextra -MMD -MP

# Build for debugging by default, or release/optimized
DEBUG	?= 1
ifeq	($(DEBUG),1)
	CXXFLAGS += -Og -g3 -DDEBUG
else
	CXXFLAGS += -O3 -DNDEBUG
endif

################################################################################
# Project files
################################################################################

C_SRCS	= calc.cpp driver.cpp math.cpp parser.cpp symbol.cpp token.cpp
SRCS	= $(C_SRCS) $(wildcard *.h)
OBJS	= $(C_SRCS:.cpp=.o)
DEPS	= $(C_SRCS:.cpp=.d)
EXE		= calc

.PHONY:	all clean cleanall docs help pr test

################################################################################
#	The default target...
################################################################################

all:	$(EXE) docs

################################################################################
# $(EXE) (calc)
################################################################################

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

################################################################################
# Include generated dependencies
################################################################################

-include $(DEPS)

################################################################################
# Cleanup intermediates...
################################################################################

clean:
	@rm -f $(OBJS) $(DEPS)

################################################################################
# Cleanup all targets and intermediates...
################################################################################

cleanall: clean
	@rm -rf $(EXE) docs

################################################################################
# Generate documentation
################################################################################

docs:	docs/html/index.html

docs/html/index.html:	Doxyfile $(SRCS)
	doxygen

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
	@pr $(C_SRCS) makefile | expand -4 | lpr

################################################################################
# Bring calc up to date and run some tests...
################################################################################

test: all
	./xcalc.sh

