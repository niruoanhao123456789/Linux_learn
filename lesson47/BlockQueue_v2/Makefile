CXX      := g++
CXXFLAGS := -std=c++20 -Wall -O2
LDFLAGS  := -lpthread

TARGET   := test
SRCS     := main.cc
OBJS     := $(SRCS:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
