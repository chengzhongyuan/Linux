.PHONY:all
all: mybin processsub
mybin:mybin.c
	gcc -o $@ $^ -std=c99
processsub:processsub.c
	gcc -o $@ $^ -std=c99
.PHONY:clean
clean:
	rm -f processsub mybin
