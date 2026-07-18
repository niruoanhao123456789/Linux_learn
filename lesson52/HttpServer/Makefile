CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
LDFLAGS = -lpthread

TARGET = httpserver
SRC = main.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
