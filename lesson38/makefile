all:Writer Reader
Reader:Reader.cc
	g++ -o $@ $^ -std=c++20
Writer:Writer.cc
	g++ -o $@ $^ -std=c++20

.PHONY:clean
clean:
	rm -f Reader Writer
