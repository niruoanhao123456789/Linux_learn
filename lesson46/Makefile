CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pthread
LDFLAGS  := -pthread

SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)
TARGET := $(basename $(firstword $(SRCS)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
