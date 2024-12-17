# Variables
CXX = g++
CXXFLAGS = -std=c++11
SRC = main.cpp
OUT = main

# Target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(OUT)
