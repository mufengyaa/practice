#include "tcp_client.hpp"

// ./tcp_client port ip
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "./tcp_client port(port>=1024)" << std::endl;
        exit(1);
    }
    tcp_client t(std::stoi(argv[1]),argv[2]);
    t.run();
    return 0;
}