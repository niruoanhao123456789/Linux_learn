CXX := g++
CXXFLAGS := -std=c++20 -Wall -Werror
LDFLAGS := -lpthread

TARGET := logger.out
SRC := main.cc
OBJ := $(SRC:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
