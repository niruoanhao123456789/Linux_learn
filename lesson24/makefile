Bin = myshell
Src = $(wildcard *.c)
Obj = $(Src:.c=.o)

$(Bin):$(Obj)
	gcc -o $@ $^
%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -f $(Bin) $(Obj)
