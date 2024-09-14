# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I/Library/gurobi1100/macos_universal2/include -L/Library/gurobi1100/macos_universal2/lib -lgurobi_c++ -lgurobi110 -lm -std=c++14 -O3
# Source file
SRCS = *.cpp

# Executable name
EXEC = comp_p_anionic_clar_num

# Build rule
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC)

# Clean rule
clean:
	rm -f $(EXEC)
