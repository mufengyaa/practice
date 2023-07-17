test:client sever
client:client.cc
	g++ -o $@ $^ -std=c++11 -g
sever:sever.cc
	g++ -o $@ $^ -std=c++11 
.PHONY:clean
clean:
	rm -f client sever
