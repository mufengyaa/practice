.PHONY:all
all:tcp_server tcp_client

tcp_server:server.cpp
	g++ -o $@ $^ -std=c++11 -lpthread
tcp_client:client.cpp
	g++ -o $@ $^ -std=c++11 

.PHONY:clean
clean:
	rm -f tcp_server tcp_client