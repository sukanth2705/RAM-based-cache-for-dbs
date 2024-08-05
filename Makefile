# Define variables
CXX = g++
LDFLAGS = -Lbuild -lcache
TARGET = test

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET):
	$(CXX) -o $(TARGET) test.cpp $(LDFLAGS)

# Rule to run the executable
run: all
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)