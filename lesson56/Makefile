CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -g -O2
TARGET := select_server
SRC := main.cc

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard *.hpp)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
