#include "udp_server.hpp"
#include <cstdlib>
#include <cstdio>
using namespace std;

std::string get_time()
{
    time_t t = time(nullptr);
    struct tm *ctime = localtime(&t);

    char time_stamp[SIZE];
    snprintf(time_stamp, sizeof(time_stamp), "[%d-%d-%d %d:%d:%d]:",
             ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
             ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
    return time_stamp;
}

std::string process_info(const std::string &info)
{
    std::string time_stamp = get_time();
    std::string id = udp_server::get_id();

    std::string res = id + time_stamp + info;
    return res;
}

std::string process_command(const std::string &cmd)
{
    std::string time_stamp = get_time();
    std::string id = udp_server::get_id();

    std::string res;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == nullptr)
    {
        lg(ERROR, "popen error, errno: %d, err string: %s", errno, strerror(errno));
        return "";
    }
    char buffer[buff_size];
    memset(buffer, 0, sizeof(buffer));
    if (fgets(buffer, sizeof(buffer) - 1, fp) == nullptr)
    {
        res = id + time_stamp + "command error";
        return res;
    }

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        if (fgets(buffer, sizeof(buffer) - 1, fp) != nullptr)
        {
            res += buffer;
        }
        else
        {
            break;
        }
    }
    return res;
}
int main()
{
    udp_server s;
    // udp_server s(8080,"127.0.0.1");
    //s.run(process_info);
    s.chat();
    return 0;
}

// //./udp_server port
// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         std::cout << "./udp_server port(port>=1024)" << std::endl;
//         exit(1);
//     }
//     udp_server s(atoi(argv[1]));
//     s.run();
//     return 0;
// }
