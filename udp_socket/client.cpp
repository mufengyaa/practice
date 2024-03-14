#include "udp_client.hpp"
#include <cstdlib>
using namespace std;

int main()
{
    udp_client c;
    //udp_client c(8080,"127.0.0.1");
    //c.run();
    
    c.chat();
    return 0;
}

// //./udp_client ip port
// int main(int argc, char *argv[])
// {
//     while (argc != 3)
//     {
//         std::cout << "./udp_client ip port(port>=1024)" << std::endl;
//         exit(1);
//     }
//     udp_client s(atoi(argv[2]), argv[1]); // port,ip
//     s.run();
//     return 0;
// }
