CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Werror -O2
LDFLAGS  := -lpthread

TARGET   := ringqueue
SRCS     := main.cc

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
