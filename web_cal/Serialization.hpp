#pragma once

#include <string>
#include <iostream>

#define protocol_sep '\n'
#define space_sep ' '

bool encode(std::string &content)
{
    // 封装报文大小
    int size = content.size();
    std::string tmp;

    tmp = std::to_string(size);
    tmp += protocol_sep;
    tmp += content;
    tmp += protocol_sep;

    content = tmp;

    return true;
}
bool decode(std::string &content, std::string &data) // 把非法的/处理完成的报文删除
{
    size_t left = content.find(protocol_sep);
    if (left == std::string::npos) // 不完整的报文
    {
        return false;
    }
    size_t right = content.find(protocol_sep, left + 1);
    if (right == std::string::npos) // 不完整的报文
    {
        return false;
    }

    // 拆出size
    std::string size_arr = content.substr(0, left);
    if (size_arr[0] < '0' || size_arr[0] > '9') // 注意size_arr里存放的不一定是数字
    {
        content.erase(0, size_arr.size() + 1); // 包括分隔符
        return false;
    }
    int size = std::stoi(size_arr);

    if (right - left != size + 1) // 错误的报文 -- right-left-1是实际有效长度,而size是理论有效长度,如果二者不匹配,说明封装上就有问题/传数据有问题
    {
        content.erase(0, size_arr.size() + 1 + right - left); // 两个分隔符+数字长度+实际数据长度
        return false;
    }

    data = content.substr(left + 1, size);        // 截断size长度的数据(这个就是完整的有效数据)
    content.erase(0, size + size_arr.size() + 2); // 两个分隔符+数字的长度
    return true;
}

// class request
// {
// public:
//     void serialize(std::string &content)
//     {
//         content+="=";
//     }
//     void deserialize(const std::string &data){

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