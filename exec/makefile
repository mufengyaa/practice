.PHONY:all
all:code test

test:test.c
	gcc -o $@ $^
code:code.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f code test
