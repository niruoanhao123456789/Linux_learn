all: myexec myproc
Src = $(wildcard *.c)
Obj = $(Src:.c=.o)

myexec:$(Obj)
	gcc -o $@ $^
%.o:%.c
	gcc -c $<

myproc:myproc.cc
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f myexec myproc $(Obj)