libmymath.a:myadd.o mysub.o
	ar -rc $@ $^
myadd.o:myadd.c
	gcc -c myadd.c -o myadd.o
mysub.o:mysub.c
	gcc -c mysub.c -o mysub.o

.PHONY:output	
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib
	cp -f *.a mylib/lib 
	cp -f *.h mylib/include

