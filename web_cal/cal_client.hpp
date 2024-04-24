#include <signal.h>

#include "socket.hpp"
#include "Serialization.hpp"
#include "help.hpp"

static MY_SOCKET my_socket;

// 网络客户端
class client
{
public:
    client(const uint16_t port, const std::string &ip)
        : port_(port), ip_(ip)
    {
    }
    ~client()
    {
        my_socket.Close();
    }
    void run()
    {
        init();
        while (true)
        {
            bool ret = my_socket.Connect(ip_, port_);
            if (!ret)
            {
                continue;
            }
            std::string tmp, in_buffer;
            char buffer[buff_size];

            while (true)
            {
                std::cout << "please enter:";
                getline(std::cin, tmp); // 读取用户输入 //"a op b"

                encode(tmp); //"size\nbuffer\n"
                int n = write(my_socket.get_fd(), tmp.c_str(), tmp.size());
                if (n < 0)
                {
                    continue;
                }
                memset(buffer, 0, sizeof(buffer));
                int ret = read(my_socket.get_fd(), buffer, sizeof(buffer)); //->"size"\n"result code"\n

                if (ret > 0)
                {
                    in_buffer += buffer;
                    //std::cout << "in_buffer: " << in_buffer;
                    while (true) // 处理多份数据
                    {
                        std::string data;
                        bool ret = decode(in_buffer, data); //"result code"
                        if (!ret)                           // 不是完整的报文
                        {
                            break;
                        }
                        //std::cout << "data: " << data;

                        response res;
                        res.deserialize(data);
                        if (res.err_code_ == 0)
                        {
                            std::cout << "result : " << res.result_ << std::endl;
                        }
                        else
                        {
                            std::map<int, std::string> err;
                            init_err(err);
                            std::cout << "error : " << err[res.err_code_] << std::endl;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

private:
    void init()
    {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        my_socket.Socket();
    }

public:
    uint16_t port_;
    std::string ip_;
};