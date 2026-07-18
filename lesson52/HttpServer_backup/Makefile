CC = g++
TARGET = httpserver
SRC = main.cc
CFLAGS = -std=c++20 -Wall -Wextra -g
LIBS = -lpthread

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: clean
