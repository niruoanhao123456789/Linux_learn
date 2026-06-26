CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 -g
LDFLAGS  := -lpthread

SERVER_SRC := UdpEchoServer.cc
CLIENT_SRC := UdpEchoClient.cc

SERVER_BIN := udp_echo_server
CLIENT_BIN := udp_echo_client

.PHONY: all clean

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
