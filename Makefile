# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I/Library/gurobi1100/macos_universal2/include -L/Library/gurobi1100/macos_universal2/lib -lgurobi_c++ -lgurobi110 -lm -I include/ -std=c++14

# Source file
SRCS = src/*.cpp

# Executable name
EXEC = build/comp_anionic_clar_num

# Build rule
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC)

# Clean rule
clean:
	rm -f $(EXEC)
