CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.cpp) \
      $(wildcard src/server/*.cpp) \
      $(wildcard src/http/*.cpp) \

TARGET = build/server

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/main.cpp $(filter-out src/main.cpp,$(SRC)) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all run clean

