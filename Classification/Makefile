# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

# Source files
SOURCES = cpp-classification/src/main.cpp cpp-classification/src/FileReader.cpp cpp-classification/src/Individual.cpp cpp-classification/src/Universe.cpp

# Target executable
TARGET = classifier

# Default target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET) 2>/dev/null || true

.PHONY: run clean