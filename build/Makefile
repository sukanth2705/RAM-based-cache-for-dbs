# Define variables
CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra
LDFLAGS = -shared
LIB_NAME = libcache.so
SRC = ../core/core.cpp
OBJ = core.o

# Default target
all: $(LIB_NAME)

# Rule to build the shared library
$(LIB_NAME): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ)

# Rule to compile the source file to an object file
$(OBJ):
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(LIB_NAME)
