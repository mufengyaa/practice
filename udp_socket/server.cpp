#include "udp_server.hpp"

int main()
{
    udp_server s;
    //udp_server s(8080,"127.0.0.1");
    s.run();
    return 0;
}
