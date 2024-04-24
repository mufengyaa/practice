.PHONY:all
all:cal_server cal_client

Lib=-ljsoncpp
Flag=-DMYSELF=1

cal_server:server.cpp
	g++ -o $@ $^ -std=c++11 $(Lib) $(Flag)
cal_client:client.cpp
	g++ -o $@ $^ -std=c++11 $(Lib) $(Flag)

.PHONY:clean
clean:
	rm -f cal_server cal_client
