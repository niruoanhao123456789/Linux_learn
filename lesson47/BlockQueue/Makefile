CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2
LDFLAGS := -lpthread

TARGET := main
SRCS := main.cc

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
