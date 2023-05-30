test:test.c
	gcc -o $@ $^ 
refresh:refresh.c
	gcc -o $@ $^ -std=c99

.PHONY:clean_test
clean_test:
	rm -rf test

.PHONY:clean_refresh
clean_refresh:
	rm -rf refresh
