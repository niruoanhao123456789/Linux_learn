Bin = Process.exe
#Src = $(shell ls *.c)
Src = $(wildcard *.c)
Obj = $(Src:.c=.o)

Echo = echo
cc = gcc
Rm = rm -f
Flags=-c -Wall
LD_Flags=-o

$(Bin):$(Obj)
	@echo "进行链接... $(Obj) -> $(Bin)"
	@$(cc) $(LD_Flags) $@ $^
%.o:%.c
	@echo "进行编译... $< -> $@"
	@$(cc) $(Flags) $<
.PHONY:clean
clean:
	rm -f $(Obj) $(Bin) 	
.PHONY:Debug
Debug:
	@echo"Debug Bin:$(Bin)"
	@echo"Debug Obj:$(Obj)"
#code:code.o
#	gcc code.o -o code
#code.o:code.s
#	gcc -c code.s -o code.o
#code.s:code.i
#	gcc -S code.i -o code.s
#code.i:code.c
#	gcc -E code.c -o code.i
#
#.PHONY:clean
#clean:
#	rm -f code.i code.s code.o code
