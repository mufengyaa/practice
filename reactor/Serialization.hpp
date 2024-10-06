#pragma once

#include <string>
#include <iostream>
#include <jsoncpp/json/json.h>

#define protocol_sep '\n'
#define space_sep ' '

bool encode(std::string &content)
{
    // 封装报文大小
    // std::cout << "content : " << content;
    int size = content.size();
    // std::cout << "size : " << size;
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
    // std::cout << "content : " << content;
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

    // // 这样会导致万一有效载荷结尾正好有我们定义的分隔符,就会导致判断为错误报文,因为right指向少了一个字符

    // if (right - left != size + 1) // 错误的报文 -- right-left-1是实际有效长度,而size是理论有效长度,如果二者不匹配,说明封装上就有问题/传数据有问题
    // {
    //     // std::cout << "left:" << left << "right:" << right << "size:" << size;
    //     content.erase(0, size_arr.size() + 1 + right - left); // 两个分隔符+数字长度+实际数据长度
    //     return false;
    // }

    // 改成下面这样,宽泛地判断即可(这样就不需要修改分隔符了,只要确保有效数据里不含分隔符即可,而不用考虑序列化后会不会包含)
    if (size + size_arr.size() + 2 > content.size()) // 如果有效载荷理论长度+size长度+两个分隔符>当前报文长度,说明此时报文不完整
    {
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
#ifdef MYSELF
        //-> "result_ err_code_"
        content = std::to_string(result_);
        content += " ";
        content += std::to_string(err_code_);
#else
        Json::Value root;
        root["result"] = result_;
        root["err_code"] = err_code_;
        // Json::FastWriter fw;
        Json::StyledWriter fw;
        content = fw.write(root); // 这里生成的string对象里,末尾有\n,所以我们需要更换分隔符,否则代码会出错
                                  // std::cout << "content: " << content;
#endif
    }
    bool deserialize(const std::string &data)
    {
#ifdef MYSELF
        //"result_ err_code_" -> result_,err_code_
        size_t pos = data.find(space_sep);
        if (pos == std::string::npos)
        {
            return false;
        }
        result_ = std::stoi(data.substr(0, pos));
        err_code_ = std::stoi(data.substr(pos + 1));
        return true;
#else
        Json::Value root;
        Json::Reader r;
        if (!r.parse(data, root))
        {
            return false;
        }
        result_ = root["result"].asInt();
        err_code_ = root["err_code"].asInt();
        return true;
#endif
    }

public:
    int result_;
    int err_code_;
};