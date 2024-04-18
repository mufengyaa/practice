.PHONY:all
all:cal_server cal_client

cal_server:server.cpp
	g++ -o $@ $^ -std=c++11 -lpthread
cal_client:client.cpp
	g++ -o $@ $^ -std=c++11 

.PHONY:clean
clean:
	rm -f cal_server cal_client