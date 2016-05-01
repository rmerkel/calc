# @file makefile
#
# @brief makefile for calc (calculator) 
#

#
# Common C++ options
#

CXXFLAGS+=-std=c++11 -Wall -Wextra

#
# Debug (default) or release (DEBUG=0)?
#

DEBUG	?= 1
ifeq	($(DEBUG),1)
	CXXFLAGS += -Og -g3 -DDEBUG
else
	CXXFLAGS += -O3 -DNDEBUG
endif

#
# Project files
#

SRCS	=  calc.cpp
OBJS	= $(SRCS:.cpp=.o)
EXE	= calc

.PHONY:	all clean cleanall docs help

all:	$(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

#
# Delete intermeadiates...
#

clean:
	rm -r $(OBJS)

#
# Delete all targets and intermeadiate files...
#

cleanall: clean
	rm -r $(EXE)

#
# Generate documentation
#

docs:
	doxygen

#
# Print a help message...
#

help:
	@echo "Usage:  make [DEBUG=0|1] targets..."
	@echo ""
	@echo "DEBUG=1, (default), builds a debug image and 0 builds a release."
	@echo ""
	@echo "Targets:"
	@echo "    all     - to build calc (default)"
	@echo "    calc    - to build the calculator"
	@echo "    clean   - to delete intermediates..."
	@echo "    cleanll - to delete all targets and intermediates..."
	@echo "    docs    - generate documentation..."
	@echo "    help    - prints this message"
	@echo ""
