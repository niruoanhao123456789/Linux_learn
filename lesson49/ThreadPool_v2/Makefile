CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -g
LDFLAGS  := -lpthread
TARGET   := main
SRCS     := main.cc
OBJDIR   := obj
OBJS     := $(SRCS:%.cc=$(OBJDIR)/%.o)

.PHONY: all clean rebuild run

all: $(TARGET)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

rebuild: clean all
