#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#define protocol_sep "\r\n"
#define blank_sep ' '
#define root_path "./root_page"
#define def_file "root.html"

std::string get_page(std::string path)
{
    std::string content, tmp;
    std::ifstream in(path.c_str());
    if (!in.is_open())
    {
        return "";
    }
    while (std::getline(in, tmp))
    {
        content += tmp;
    }
    in.close();

    // printf("content : %s", content.c_str());
    return content;
}
std::string b_get_page(std::string path)
{
    std::ifstream in(path.c_str(), std::ios_base::binary);
    if (!in.is_open())
    {
        return "";
    }
    in.seekg(0, std::ios_base::end);
    auto len = in.tellg();
    in.seekg(0, std::ios_base::beg);

    std::string content;
    content.resize(len);

    in.read((char *)content.c_str(), content.size());
    in.close();

    return content;
}

class request
{
public:
    request()
        : type_(""), url_(""), version_(""), text_(""), path_(""), code_(0), suffix_("") {}
    bool deserialize(std::string &content)
    {
        std::string tmp;
        size_t left = 0;
        while (true)
        {
            size_t pos = content.find(protocol_sep, left); //"/r/n"的第一个字符'/r'
            if (pos == std::string::npos)
            {
                return false;
            }

            tmp = content.substr(left, pos - left); // 左闭右开
            left = pos + 2;                         // 跳过/r/n
            if (tmp.empty())                        // 读到空行
            {
                break;
            }
            else
            {
                title_.push_back(tmp);
            }
        }

        // 判断是否有正文
        std::string comp = "Content-Length: ";
        bool is_find = false;
        int size = 0;
        for (auto &it : title_)
        {
            size_t pos = it.find(comp);
            if (pos != std::string::npos)
            {
                ssize_t right = it.find(protocol_sep);
                std::string s_size = it.substr(pos + comp.size(), right - pos - comp.size());
                size = stoi(s_size);
                is_find = true;
                break;
            }
        }
        if (!is_find)
        { // 没有Content-Length字段
            content.erase(0, left);
        }
        else
        {
            text_ = content.substr(left, size);
            content.erase(0, left + size);
        }

        handle_path();

        return true;
    }

private:
    void handle_path() // 构建访问资源的路径
    {
        // 细分请求行
        std::string request_line = title_[0];
        std::stringstream ss(request_line);
        ss >> type_ >> url_ >> version_;

        // 构建路径
        path_ = root_path;
        if (url_ == "/")
        {
            path_ += "/";
            path_ += def_file;
        }
        else if (url_ == "/302")
        {
            code_ = 302; // 这里设置为重定向到百度页面,所以不能走本地读取
            return;
        }
        else
        {
            path_ += url_;
        }

        // 判断该资源是否存在
        if (get_page(path_).empty())
        {
            code_ = 404; // 需要告诉响应,这里发生了404错误
            path_ = root_path;
            path_ += "/";
            path_ += "404_err.html";
        }

        // 拿到资源后缀
        size_t pos = path_.rfind(".");
        if (pos == std::string::npos)
        {
            suffix_ = ".html";
        }
        else
        {
            suffix_ = path_.substr(pos);
        }
    }

public:
    std::string type_;
    std::string url_;
    std::string path_;
    int code_;
    std::string suffix_;
    std::string version_;
    std::vector<std::string> title_;
    std::string text_;
};

class response
{
public:
    response()
        : version_(""), code_(0), desc_(""), text_("") {}
    void serialize(std::string &content)
    {
        // 响应行
        std::string header = version_;
        header += blank_sep;
        header += std::to_string(code_);
        header += blank_sep;
        header += desc_;
        header += protocol_sep;

        // 响应报头
        std::string attribute;
        for (auto &it : title_)
        {
            attribute += it;
            attribute += protocol_sep;
        }

        content = header + attribute + protocol_sep + text_;
    }

public:
    std::string version_;
    int code_;
    std::string desc_;
    std::vector<std::string> title_;
    std::string text_;
};