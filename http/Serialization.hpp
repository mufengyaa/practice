#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#define protocol_sep "\r\n"
#define blank_sep ' '

class request
{
public:
    request()
        : type_(""), url_(""), version_(""), text_("") {}
    bool deserialize(std::string &content)
    {
        std::string tmp;
        size_t left = 0;
        while (true)
        {
            size_t pos = content.find(protocol_sep, left);
            if (pos == std::string::npos)
            {
                return false;
            }

            tmp = content.substr(left, pos - left); // 左闭右开
            left = pos + 2;
            if (tmp.empty()) // 读到空行
            {
                break;
            }
            else
            {
                title_.push_back(tmp);
            }
        }

        // 细分请求行
        std::string request_line = title_[0];
        std::stringstream ss(request_line);
        ss >> type_ >> url_ >> version_;

        // 如果有正文的话
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
            content.erase(0, left + 2);
        }
        else
        {
            text_ = content.substr(left + 2, size);
            content.erase(0, left + 2 + size);
        }
        return true;
    }

public:
    std::string type_;
    std::string url_;
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