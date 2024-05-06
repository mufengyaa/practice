#pragma once

#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <functional>
#include <unordered_map>

#include "socket.hpp"
#include "Serialization.hpp"

static MY_SOCKET my_socket;

#define buff_size 1024 * 30

class http_server
{
public:
    http_server(const uint16_t port, const std::string &ip = "0.0.0.0")
        : port_(port), ip_(ip)
    {
        content_type_[".html"] = "text/html";
        content_type_[".png"] = "image/png";
        content_type_[".jpg"] = "image/jpeg";
        content_type_[".jpeg"] = "image/jpeg";
    }
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
                        buffer[n] = 0;
                        in_buffer += buffer; // 连续读取
                        lg(INFO, "get request: \n%s", in_buffer.c_str());

                        // 构建请求
                        request req;
                        req.deserialize(in_buffer);

                        //lg(DEBUG, "path: %s ,url: %s ", (req.path_).c_str(), (req.url_).c_str());

                        // 构建响应
                        response res;
                        handle_response(res, req);

                        // 响应序列化
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
    void handle_response(response &res, request &req)
    {
        int code = req.code_;
        std::string path = req.path_;
        std::string content_type_data = content_type_[req.suffix_];
        //lg(DEBUG, "content_type_data: %s", content_type_data.c_str());

        res.version_ = "HTTP/1.1";
        if (code == 302)
        {
            res.code_ = 302;
            res.desc_ = "Found";

            std::string cl = "Location: ";
            cl += "https://www.qq.com";
            (res.title_).push_back(cl);
            return ;
        }

        if (code == 404)
        {
            res.code_ = 404;
            res.desc_ = "Not Found";
        }
        else
        {
            res.code_ = 200;
            res.desc_ = "OK";
        }

        // 将读取网页和图片资源的方式分开
        if (req.suffix_ == ".html")
        {
            res.text_ = get_page(path);
            //lg(DEBUG, "text: %s", (res.text_).c_str());
        }
        else
        {
            res.text_ = b_get_page(path);
        }

        //  构建响应报头
        std::string cl = "Content-Length: ";
        cl += std::to_string((res.text_).size());
        //lg(DEBUG, "text_size: %d", (res.text_).size());
        (res.title_).push_back(cl);

        cl = "Content-Type: ";
        cl += content_type_data;
        (res.title_).push_back(cl);
    }

private:
    uint16_t port_;
    std::string ip_;

    std::unordered_map<std::string, std::string> content_type_;
};
