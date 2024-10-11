// 对网页信息去标签和数据清洗,生成一份新文件

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <boost/filesystem.hpp>

#include "asssistance.hpp"

#define source_file_path "../input"
#define target_path "../data/data.txt"
#define delimiter '\3'

namespace fs = boost::filesystem;

class Parser
{
    std::vector<ns_helper::doc_info> docs_;

public:
    Parser() {}
    ~Parser() {}
    bool parser(const std::string &path)
    {
        fs::path directory(path);
        if (fs::exists(directory) && fs::is_directory(directory))
        {
            fs::directory_iterator begin(directory), end;
            for (fs::directory_iterator file = begin; file != end; ++file)
            {
                std::string data;
                if (fs::is_regular_file(*file) && file->path().extension() == ".html")
                {
                    // std::cout << file->path().string() << std::endl;
                    ns_helper::read_file(file->path().string(), &data); // 读出一个html文件的内容
                    ns_helper::doc_info document;
                    if (!analysis(data, &document, file->path().string()))
                    {
                        std::cerr << "analysis failed" << std::endl;
                    }
                    // // for debug
                    // std::cout << "title: " << document.title_ << std::endl;
                    // std::cout << "content: " << document.content_ << std::endl;
                    // std::cout << "url: " << document.url_ << std::endl;
                    // break;

                    // 成功格式化文档
                    docs_.push_back(std::move(document));
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }
    void write(const std::string &path, char sep)
    {
        std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
        if (!out.is_open())
        {
            std::cerr << "file: " << path.c_str() << " open failed" << std::endl;
        }
        std::string line;
        for (const auto &it : docs_)
        {
            out.write(it.title_.c_str(), it.title_.size());
            out << delimiter;
            out.write(it.content_.c_str(), it.content_.size());
            out << delimiter;
            out.write(it.url_.c_str(), it.url_.size());
            out << '\n';
        }
        out.close();
    }

private:
    bool analysis(const std::string &data, ns_helper::doc_info *result, const std::string &path)
    {
        // 提取title
        std::string t = "<title>";
        ssize_t begin = data.find(t);
        if (begin == std::string::npos)
        {
            return false;
        }
        begin += t.size();
        ssize_t end = data.find("</title>");
        if (end == std::string::npos)
        {
            return false;
        }

        result->title_ = data.substr(begin, end - begin);
        // std::cout << result->title_ << std::endl;

        // 提取content,也就是去掉标签
        enum state
        {
            label,
            content
        };
        int flag = label;
        for (auto c : data)
        {
            if (c == '<')
            {
                flag = label;
            }
            else if (c == '>')
            {
                flag = content;
            }
            if (flag == content) // 分成两种情况,进入正文/新标签
            {
                if (c == '<') // 进入新标签
                {
                    flag = label;
                }
                else
                {
                    if (c == '\n')
                    {
                        c = ' ';
                    }
                    (result->content_) += c;
                }
            }
            else
            {
                continue;
            }
        }
        // std::cout << result->content_ << std::endl;

        // 构建url
        std::string head = "https://www.boost.org/doc/libs/1_78_0/doc/html";
        t = source_file_path;
        std::string tail = path.substr(t.size());
        result->url_ = head + tail;
        // std::cout << result->url_ << std::endl;

        return true;
    }
};

void work()
{
    Parser p;
    if (!p.parser(source_file_path))
    {
        std::cerr << "parser falied" << std::endl;
    }
    p.write(target_path, delimiter);
}
int main()
{
    work();
    return 0;
}