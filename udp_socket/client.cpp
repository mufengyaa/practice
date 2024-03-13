#include"udp_client.hpp"

int main()
{
    udp_client c;
    //udp_client c(8080,"127.0.0.1");
    c.run();
    return 0;
}
