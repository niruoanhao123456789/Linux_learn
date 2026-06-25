CXX = g++
CXXFLAGS = -std=c++20 -Wall -Werror -O2 -lpthread

TARGET_SERVER = DictServer
TARGET_CLIENT = DictClient

.PHONY: all
all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): DictServer.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

$(TARGET_CLIENT): DictClient.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

.PHONY: rebuild
rebuild: clean all
