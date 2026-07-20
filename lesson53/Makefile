TARGET = http_server
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
LDFLAGS = -lpthread

$(TARGET): main.cc HttpServer.hpp HttpProtocol.hpp TcpServer.hpp Socket.hpp InetAddr.hpp Logger.hpp Mutex.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cc $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
