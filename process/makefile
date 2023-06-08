zombie:zombie.c
	gcc zombie.c -o zombie
zombie1:zombie1.c
	gcc -o $@ $^
orphan:orphan.c
	gcc orphan.c -o orphan
code:code.c
	gcc $^ -o $@ -g
.PHONY:clean_zombie
clean_zombie:
	rm -f zombie
.PHONY:clean_zombie1
clean_zombie1:
	rm -f zombie1
.PHONY:clean_orphan
clean_orphan:
	rm -f orphan
.PHONY:clean_code
clean_code:
	rm -f code
