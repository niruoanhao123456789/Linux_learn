CXX      := g++
CXXFLAGS := -std=c++20 -Wall -O2
LDFLAGS  := -lpthread
TARGET   := ringqueue_v2
SRCS     := main.cc
OBJS     := $(SRCS:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
