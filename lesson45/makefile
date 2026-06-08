CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = testThread
SRCS = main.cc
OBJS = $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc Thread.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean