#include "http_server.hpp"

// ./tcp_server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "./http_server port(port>=1024)" << std::endl;
        exit(1);
    }
    http_server t(std::stoi(argv[1]));
    t.run();
    return 0;
}