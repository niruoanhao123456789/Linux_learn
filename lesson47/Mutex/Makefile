CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
LDFLAGS  := -lpthread

TARGET   := Mutex
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
