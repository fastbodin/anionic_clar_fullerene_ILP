# Compiler
CXX = g++-14

# Compiler flags
GUROBI = -I/Library/gurobi1103/macos_universal2/include -L/Library/gurobi1103/macos_universal2/lib -lgurobi_c++ -lgurobi110 -lm
INCLUDE = -I ../include/
CXX_FLAGS = -std=c++14 -O3

# Directories
SRC_DIR = ../src
TEST_DIR = src

# Executable name
EXEC = build/unit_test

# Source and test files
SRC_FILES = $(wildcard $(SRC_DIR)/dual.cpp $(SRC_DIR)/lp.cpp $(SRC_DIR)/read_and_print.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Build all test files
all: $(EXEC)

# Build rule
$(EXEC): $(SRC_FILES) $(TEST_FILES)
	$(CXX) $(GUROBI) $(INCLUDE) $(CXX_FLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(EXEC)
