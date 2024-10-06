# code:code.cpp
# 	g++ -o $@ $^ -std=c++11 -lpthread
# .PHONY:clean
# clean:
# 	rm -f code

libmy.a:Task.o
	ar -rc libmy.a Task.o
Task.o:Task.cpp
	g++ -c Task.cpp -o Task.o -std=c++11
.PHNOY:mylib
mylib:
	mkdir -p mylib/lib
	mkdir -p mylib/include
	mv *.a mylib/lib
	cp -rf *.hpp mylib/include
	rm *.o -f
.PHONY:clean
clean:
	rm *.o mylib libmy.a -rf 