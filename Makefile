HEADERS = EquationParser.hpp EquationSolver.hpp
OBJS = EquationParser.o EquationSolver.o main.o

#
# C++ Compiler
CXX := g++
#
#
# Flags
#
#   Compiler
CXXFLAGS += -c -g -Wall -std=c++11
#
#   Linker
LDFLAGS += -g
#
#   Library
#LIBFLAGS :=
#
#   Include
#INCLUDEFLAGS :=
#
#
# Program name
EXE := LinearEquationSolver
#
#
# Directories
#
#   Headers
#HEADERDIR := include
#
#   Sources
#SRCDIR := src
#
#   Objects
OBJDIR := obj
#
#   Binary
#BINDIR := bin
#
#
################################################################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)

CXXFLAGS += $(INCLUDEFLAGS) -I$(HEADERDIR)
LDFLAGS += $(LIBFLAGS)

.PHONY: all clean distclean

all: $(EXE)

$(EXE): $(OBJS)
	#mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $? -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(EXE)

distclean: clean
