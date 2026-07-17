CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -O2 -g
LDFLAGS  := -lpthread

SRCSERVER := TcpExecServer.cc
SRCCLIENT := TcpExecClient.cc

TARGETSERVER := TcpExecServer
TARGETCLIENT := TcpExecClient

OBJSERVER := $(SRCSERVER:.cc=.o)
OBJCLIENT := $(SRCCLIENT:.cc=.o)

.PHONY: all clean

all: $(TARGETSERVER) $(TARGETCLIENT)

$(TARGETSERVER): $(OBJSERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGETCLIENT): $(OBJCLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJSERVER) $(OBJCLIENT) $(TARGETSERVER) $(TARGETCLIENT)
