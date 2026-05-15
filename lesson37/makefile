all:client server
client:client.cc
	g++ -o $@ $^ -std=c++20
server:server.cc
	g++ -o $@ $^ -std=c++20

.PHONY:clean
clean:
	rm -f client server fifo