CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Werror -Wextra -pedantic
LDFLAGS  :=
LDLIBS   := -lpthread

SRC_SVR  := TcpEchoServer.cc
SRC_CLI  := TcpEchoClient.cc

TARGETS  := TcpEchoServer TcpEchoClient

.PHONY: all clean rebuild

all: $(TARGETS)

TcpEchoServer: $(SRC_SVR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

TcpEchoClient: $(SRC_CLI)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	$(RM) $(TARGETS)

rebuild: clean all
