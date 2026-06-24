CXX       = g++
CXXFLAGS  = -std=c++20 -Wall -Werror

TARGET_SERVER = ChatServer
TARGET_CLIENT = ChatClient

SRC_SERVER = ChatMain.cc
SRC_CLIENT = ChatClient.cc

.PHONY: all clean

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)
