// 对网页信息去标签和数据清洗,生成一份新文件

#pragma once

#include <utility>
#include <boost/filesystem.hpp>

#include "assistance.hpp"

namespace fs = boost::filesystem;

class Parser
{
    std::vector<ns_helper::doc_info> docs_;

public:
    Parser() {}
    ~Parser() {}
    bool parser(const std::string &path, const std::string &output_path)
    {
        std::ofstream out(output_path, std::ios_base::out | std::ios_base::binary);
        if (!out.is_open())
        {
            std::cerr << "file: " << output_path.c_str() << " open failed" << std::endl;
            return false;
        }

        fs::path directory(path);
        if (fs::exists(directory) && fs::is_directory(directory))
        {
            fs::recursive_directory_iterator begin(directory), end;
            for (fs::recursive_directory_iterator file = begin; file != end; ++file)
            {
                if (fs::is_regular_file(*file) && file->path().extension() == ".html")
                {
                    std::string data;
                    ns_helper::read_file(file->path().string(), &data);
                    ns_helper::doc_info document;
                    // 边解析边写入文件
                    if (analysis(data, &document, file->path().string()))
                    {
                        out.write(document.title_.c_str(), document.title_.size());
                        out << delimiter;
                        out.write(document.content_.c_str(), document.content_.size());
                        out << delimiter;
                        out.write(document.url_.c_str(), document.url_.size());
                        out << '\n';
                    }
                }
            }
        }
        else
        {
            return false;
        }

        out.close();
        std::cout << "write success" << std::endl;
        return true;
    }

    // void write(const std::string &path, char sep) // 一起写
    // {
    //     std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
    //     if (!out.is_open())
    //     {
    //         std::cerr << "file: " << path.c_str() << " open failed" << std::endl;
    //     }
    //     std::string line;
    //     for (const auto &it : docs_)
    //     {
    //         out.write(it.title_.c_str(), it.title_.size());
    //         out << delimiter;
    //         out.write(it.content_.c_str(), it.content_.size());
    //         out << delimiter;
    //         out.write(it.url_.c_str(), it.url_.size());
    //         out << '\n';
    //     }
    //     out.close();
    //     std::cout << "write success" << std::endl;
    // }

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
            LABLE,
            CONTENT
        };
        enum state s = LABLE;
        for (char c : data)
        {
            switch (s)
            {
            case LABLE:
                if (c == '>')
                    s = CONTENT;
                break;
            case CONTENT:
                if (c == '<')
                    s = LABLE;
                else
                {
                    // 我们不想保留原始文件中的\n,因为我们想用\n作为html解析之后文本的分隔符
                    if (c == '\n')
                        c = ' ';
                    result->content_ += c;
                }
                break;
            default:
                break;
            }
        }
        // std::cout << result->content_ << std::endl;

        // 构建url
        std::string head = "https://www.boost.org/doc/libs/1_86_0/doc/html/";
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
    if (!p.parser(source_file_path, target_path))
    {
        std::cerr << "parser falied" << std::endl;
    }
}
int main()
{
    work();
    return 0;
}