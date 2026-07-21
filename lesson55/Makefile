CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
LDFLAGS = -lpthread

TARGET1 = tcp_client
TARGET2 = tcp_exec_server

all: $(TARGET1) $(TARGET2)

$(TARGET1): TcpClient.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(TARGET2): TcpExecServer.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET1) $(TARGET2)

.PHONY: all clean
