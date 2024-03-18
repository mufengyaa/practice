#include "tcp_server.hpp"

// ./tcp_server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "./tcp_server port(port>=1024)" << std::endl;
        exit(1);
    }
    tcp_server t(std::stoi(argv[1]));
    t.run_pthread_pool();
    return 0;
}