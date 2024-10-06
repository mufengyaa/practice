#include"server.hpp"

int main(){
    epoll_server es;
    es.start();
    return 0;
}