#pragma once

#include <string>

#define protocol_sep '\n'
#define space_sep ' '

std::string encode(const std::string &content)
{
    // 封装报文大小
    int size = content.size();

    std::string ret = std::to_string(size);
    ret += protocol_sep;
    ret += content;
    ret += protocol_sep;

    return ret;
}
bool decode(std::string &content)
{
    size_t left = content.find(protocol_sep);
    if (left == std::string::npos) // 不完整的报文
    {
        return false;
    }

    std::string size_arr = content.substr(0, left);
    int size = std::stoi(size_arr);

    size_t right = content.rfind(protocol_sep);
    if (right - left != size + 1) // 不完整的报文WS
    {
        return false;
    }

    content = content.substr(left + 1, right); // 截断中间部分(这个就是完整的有效数据)
    if (!content.empty() && content.back() == '\n') //插眼,很奇怪为什么还会有\n
    {
        content.pop_back(); // 如果子串最后一个字符是换行符，则移除
    }
    return true;
}

// class request
// {
// public:
//     void Serialize(std::string &content)
//     {
//         encode(content);

//     }

// public:
// };

class response
{
public:
    response(int result, int err)
        : result_(result), err_code_(err) {}
    response() {} // 用于接收反序列化的对象
    ~response() {}

    void serialize(std::string &content)
    {
        //-> "result_ err_code_"
        content = std::to_string(result_);
        content += " ";
        content += std::to_string(err_code_);
    }
    bool deserialize(const std::string &data)
    {
        //"result_ err_code_" -> result_,err_code_
        size_t pos = data.find(space_sep);
        if (pos == std::string::npos)
        {
            return false;
        }
        result_ = std::stoi(data.substr(0, pos));
        err_code_ = std::stoi(data.substr(pos + 1));
        return true;
    }

public:
    int result_;
    int err_code_;
};