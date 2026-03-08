Bin = process_bar
Src = $(wildcard *.c)
Obj = $(Src:.c=.o)
cc = gcc

$(Bin):$(Obj)
	@echo "$^ linking to $@"
	@$(cc) -o $@ $^
%.o:%.c
	@echo "compling $< to $@"
	@$(cc) -c $<

.PHONY:clean
clean:
	@echo "Clean project done"
	@rm -f $(Obj) $(Bin)

.PHONY:debug
debug:
	@echo "$(Bin)"
	@echo "$(Obj)"
	@echo "$(Src)"
	@echo "$(cc)"
