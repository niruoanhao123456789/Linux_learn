CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -lpthread
TARGET   := thread_pool
SRCS     := main.cc

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
