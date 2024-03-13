.PHONY:all
all:udp_server udp_client

udp_server:server.cpp
	g++ -o $@ $^ -std=c++11
udp_client:client.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f udp_server udp_client