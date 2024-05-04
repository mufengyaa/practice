#pragma once

#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <functional>

#include "socket.hpp"
#include "Serialization.hpp"

static MY_SOCKET my_socket;

#define buff_size 1024 * 30
#define root_path "./root_page"
#define def_file "root.html"

class http_server
{
public:
    http_server(const uint16_t port, const std::string &ip = "0.0.0.0")
        : port_(port), ip_(ip) {}
    ~http_server() {}
    void run()
    {
        init();
        while (true)
        {
            uint16_t client_port;
            std::string client_ip;
            lg(DEBUG, "accepting ...");
            int sockfd = my_socket.Accept(client_ip, client_port);
            if (sockfd == -1)
            {
                continue;
            }
            lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip.c_str(), client_port);

            int ret = fork();
            if (ret == 0)
            {
                my_socket.Close();
                char buffer[buff_size];
                std::string in_buffer;

                while (true)
                {
                    memset(buffer, 0, sizeof(buffer));
                    int n = read(sockfd, buffer, sizeof(buffer)); //"size"\n"a op b"\n
                    if (n > 0)
                    {
                        bool code_302 = false;
                        buffer[n] = 0;
                        lg(INFO, "get request");
                        in_buffer += buffer; // 连续读取
                        lg(INFO, "%s", in_buffer.c_str());
                        request req;
                        req.deserialize(in_buffer);

                        // 构建访问资源的路径
                        std::string def_page = root_path;
                        if (req.url_ == "/")
                        {
                            def_page += "/";
                            def_page += def_file;
                        }
                        else if (req.url_ == "/302")
                        {
                            code_302 = true;
                        }
                        else
                        {
                            def_page += req.url_;
                        }

                        // 构建响应
                        response res;
                        res.version_ = "HTTP/1.1";
                        if (code_302)
                        {
                            res.code_ = 302;
                            res.desc_ = "Found";

                            std::string cl = "Location: ";
                            cl += "https://www.baidu.com";
                            cl += protocol_sep;
                            (res.title_).push_back(cl);
                        }
                        else
                        {
                            std::string text = get_page(def_page);
                            if (text.empty())
                            {
                                res.code_ = 404;
                                res.desc_ = "Not Found";
                                def_page = root_path;
                                def_page += "/";
                                def_page += "404_err.html";

                                req.text_ = get_page(def_page);
                            }
                            else
                            {
                                res.code_ = 200;
                                res.desc_ = "OK";
                                res.text_ = text;
                            }

                            std::string cl = "Content-Length: ";
                            cl += std::to_string((req.text_).size());
                            cl += protocol_sep;
                            (res.title_).push_back(cl);
                        }

                        std::string content;
                        res.serialize(content);

                        write(sockfd, content.c_str(), content.size());
                    }
                    else if (n == 0)
                    {
                        lg(INFO, "%s quit", client_ip.c_str());
                        break;
                    }
                    else // 读出错误
                    {
                        break;
                    }
                }
                // lg(INFO, "fork quit");
                exit(0);
                close(sockfd);
            }
        }
    }

private:
    void init()
    {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        my_socket.Socket();
        my_socket.Bind(port_);
        my_socket.Listen();
        lg(INFO, "server init done");
    }
    std::string get_page(std::string path)
    {
        std::ifstream in(path.c_str());
        if (!in.is_open())
        {
            return "";
        }
        std::string content, tmp;
        while (std::getline(in, tmp))
        {
            content += tmp;
        }
        return content;
    }

private:
    uint16_t port_;
    std::string ip_;
};
