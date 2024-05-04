.PHONY:all
all:http_server

http_server:server.cpp
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f http_server