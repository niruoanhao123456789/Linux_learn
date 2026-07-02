CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2
LDFLAGS := -lpthread -ljsoncpp

SRCS := OnlineCalClient.cc OnlineCalServer.cc
CLIENT_SRC := OnlineCalClient.cc
SERVER_SRC := OnlineCalServer.cc
CLIENT_TARGET := OnlineCalClient
SERVER_TARGET := OnlineCalServer

.PHONY: all clean

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(SERVER_TARGET): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET)
