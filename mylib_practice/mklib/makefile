libmy.a:add.o print.o
	ar -rc libmy.a add.o print.o
add.o:add.c
	gcc -c add.c -o add.o -std=c99
print.o:print.c
	gcc -c print.c -o print.o
.PHNOY:mylib
mylib:
	mkdir -p mylib/lib
	mkdir -p mylib/include
	mv *.a mylib/lib
	cp -rf *.h mylib/include
	rm *.o -f
.PHONY:clean
clean:
	rm *.o mylib libmy.a -rf 

