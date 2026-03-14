Bin = testpipe
Src = $(wildcard *.cpp)
Obj = $(Src:.cpp=.o)

$(Bin):$(Obj)
	@g++ -o $@ $^ -std=c++20
*.o:*.cpp
	@g++ -c $<

.PHONY:clean
clean:
	@rm -f $(Bin) $(Obj)