# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

# Name of the executable
TARGET = chess_engine

# Directories
SRCDIR = src
BUILDDIR = build
# Source and header files
SOURCES = $(SRCDIR)/board.cpp $(SRCDIR)/utils.cpp
HEADERS = include/board.h include/utils.h include/constants.h

# Object files (place in the build directory)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into build directory
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILDDIR) # Ensure build directory exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
