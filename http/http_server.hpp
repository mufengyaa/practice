#pragma once

#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <functional>
#include <pthread.h>
#include <unordered_map>

#include "socket.hpp"
#include "Serialization.hpp"

static MY_SOCKET my_socket;

#define buff_size 1024 * 30

class http_server;
struct thread_data
{
    int sockfd_;
    std::string ip_;
    std::string &in_buffer_;
    http_server *this_;
};

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

            // 一个线程处理一次请求(短连接)
            pthread_t pid;
            std::string in_buffer;

            int sockfd = 0;
            int count = 5;
            do
            {
                lg(DEBUG, "accepting ...");
                sockfd = my_socket.Accept(client_ip, client_port);
                if (sockfd != -1 || --count == 0)
                {
                    break;
                }
            } while (true);
            if (sockfd == -1)
            {
                lg(ERROR, "accepting error");
            }

            lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip.c_str(), client_port);

            thread_data *td = new thread_data{sockfd, client_ip, in_buffer, this};
            lg(DEBUG, "create pthread");
            pthread_create(&pid, nullptr, entry, reinterpret_cast<void *>(td));

            // 一个进程服务一个客户端

            // lg(DEBUG, "accepting ...");
            // int sockfd = my_socket.Accept(client_ip, client_port);
            // if (sockfd == -1)
            // {
            //     continue;
            // }
            // lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip.c_str(), client_port);
            //  int ret = fork();
            //  if (ret == 0)
            //  {
            //      my_socket.Close();
            //  char buffer[buff_size];
            //  std::string in_buffer;

            // while (true)
            // {
            //     memset(buffer, 0, sizeof(buffer));
            //     int n = read(sockfd, buffer, sizeof(buffer)); //"size"\n"a op b"\n
            //     if (n > 0)
            //     {
            //         buffer[n] = 0;
            //         in_buffer += buffer; // 连续读取
            //         lg(INFO, "get request: \n%s", in_buffer.c_str());

            //         // 构建请求
            //         request req;
            //         req.deserialize(in_buffer);

            //         // lg(DEBUG, "path: %s ,url: %s ", (req.path_).c_str(), (req.url_).c_str());

            //         // 构建响应
            //         response res;
            //         handle_response(res, req);

            //         // 响应序列化
            //         std::string content;
            //         res.serialize(content);

            //         write(sockfd, content.c_str(), content.size());
            //     }
            //     else if (n == 0)
            //     {
            //         lg(INFO, "%s quit", client_ip.c_str());
            //         break;
            //     }
            //     else // 读出错误
            //     {
            //         break;
            //     }
            // }
            //     exit(0);
            //     close(sockfd);
            // }
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
        // lg(DEBUG, "content_type_data: %s", content_type_data.c_str());

        res.version_ = "HTTP/1.1";
        if (code == 302)
        {
            res.code_ = 302;
            res.desc_ = "Found";

            std::string cl = "Location: ";
            cl += "https://www.qq.com";
            (res.title_).push_back(cl);
            return;
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
            // lg(DEBUG, "text: %s", (res.text_).c_str());
        }
        else
        {
            res.text_ = b_get_page(path);
        }

        //  构建响应报头
        std::string cl = "Content-Length: ";
        cl += std::to_string((res.text_).size());
        // lg(DEBUG, "text_size: %d", (res.text_).size());
        (res.title_).push_back(cl);

        cl = "Content-Type: ";
        cl += content_type_data;
        (res.title_).push_back(cl);

        // 增加cookie
        if (!(req.text_).empty())
        {
            // lg(DEBUG, "TEXT: %s", req.text_.c_str());

            size_t left = 0, right = 0;
            while (true) // 把请求的正文部分的内容读出来
            {
                cl = "Set-Cookie: ";
                right = (req.text_).find("&", left);
                if (right == std::string::npos)
                {
                    cl += (req.text_).substr(left);
                    break;
                }
                cl += (req.text_).substr(left, right - left);
                left = right + 1;
                (res.title_).push_back(cl);
            }
        }
    }
    static void *entry(void *args)
    {
        pthread_detach(pthread_self());

        thread_data *td = reinterpret_cast<thread_data *>(args);
        int sockfd = td->sockfd_;
        std::string ip = td->ip_;
        std::string in_buffer = td->in_buffer_;
        http_server *it = td->this_;

        // 读取请求
        char buffer[buff_size];
        bool flag = true;
        request req;
        while (true) // 虽说是短连接,但也得确保读出来的内容是一个完整的请求
        {
            memset(buffer, 0, sizeof(buffer));
            int n = read(sockfd, buffer, sizeof(buffer));
            if (n > 0)
            {
                buffer[n] = 0;
                in_buffer += buffer; // 连续读取
                lg(INFO, "get request: \n%s", in_buffer.c_str());

                // 构建请求
                flag = req.deserialize(in_buffer);
                if (flag == false)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if (n == 0)
            {
                lg(INFO, "%s quit", ip.c_str());
                return nullptr;
            }
            else
            {
                lg(ERROR, "%s read error", ip.c_str());
                return nullptr;
            }
        }

        // lg(DEBUG, "path: %s ,url: %s ", (req.path_).c_str(), (req.url_).c_str());

        // 构建响应
        response res;
        it->handle_response(res, req);

        // 响应序列化
        std::string content;
        res.serialize(content);

        write(sockfd, content.c_str(), content.size());

        //  销毁资源
        delete td;
        close(sockfd);

        return nullptr;
    }

private:
    uint16_t port_;
    std::string ip_;

    std::unordered_map<std::string, std::string> content_type_;
};
