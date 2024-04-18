#pragma once

#include "Serialization.hpp"
#include <map>
#include <utility>
#include <vector>
#include <stack>
#include <iostream>

// 提供计算和解析封装报文的功能

enum
{
    ERROR_DIVIDE_BY_ZERO = 1,     // 除以零错误
    ERROR_INVALID_EXPRESSION = 2, // 无效表达式错误
    ERROR_INVALID_OPERATION = 3,  // 无效操作错误
    ERROR_MEMORY_OVERFLOW = 4,    // 内存溢出错误
    ERROR_SYNTAX_ERROR = 5        // 语法错误
};

class calculate
{
public:
    calculate()
        : err_code_(0) {}
    ~calculate() {}

    std::string cal(std::string &arr) // 封装后的报文
    {
        // 解析出表达式
        bool ret = decode(arr);
        if (!ret) // 不是完整的报文
        {
            return "";
        }

        // 计算
        response res;
        std::map<std::string, int> comp;
        confirm_priority(comp);

        std::vector<std::string> tmp = to_suffix(arr, comp);

        res.result_ = work(tmp, comp);
        res.err_code_ = err_code_;

        // 序列化响应
        std::string content;
        res.serialize(content);

        return encode(content);
    }

private:
    void confirm_priority(std::map<std::string, int> &comp)
    {
        comp.insert(std::make_pair("=", -1));

        comp.insert(std::make_pair("||", 1));

        comp.insert(std::make_pair("&&", 2));

        comp.insert(std::make_pair("==", 3));
        comp.insert(std::make_pair("!=", 3));

        comp.insert(std::make_pair(">", 4));
        comp.insert(std::make_pair("<", 4));
        comp.insert(std::make_pair("<=", 4));
        comp.insert(std::make_pair(">=", 4));

        comp.insert(std::make_pair("+", 5));
        comp.insert(std::make_pair("-", 5));

        comp.insert(std::make_pair("*", 6));
        comp.insert(std::make_pair("/", 6));
        comp.insert(std::make_pair("%", 6));

        comp.insert(std::make_pair("!", 7));
    }
    std::vector<std::string> to_suffix(const std::string arr, std::map<std::string, int> &comp)
    {
        std::string numbers("0123456789");
        std::vector<std::string> ans;
        std::stack<std::string> cal;
        for (auto i = 0; i < arr.size(); ++i)
        {
            if (arr[i] >= '0' && arr[i] <= '9')
            {
                auto pos = arr.find_first_not_of(numbers, i);
                std::string tmp_arr(arr.begin() + i, arr.begin() + pos);
                ans.push_back(tmp_arr);
                i = pos - 1;
            }
            else
            { // 符号
                if (arr[i] == '-' && arr[i + 1] >= '0' && arr[i + 1] <= '9')
                {
                    std::string tmp;
                    tmp += arr[i];
                    auto pos = arr.find_first_not_of(numbers, i + 1);
                    std::string tmp_arr(arr.begin() + i + 1, arr.begin() + pos);
                    ans.push_back(tmp + tmp_arr);
                    i = pos - 1;
                    continue;
                }
                std::string sym;
                sym += arr[i];
                if (((arr[i] == '>' || arr[i] == '<' || arr[i] == '=' || arr[i] == '!') && arr[i + 1] == '=') || (arr[i] == '|' && arr[i + 1] == '|') || (arr[i] == '&' && arr[i + 1] == '&'))
                {
                    ++i;
                    sym += arr[i];
                }

                if (sym == "(")
                {
                    cal.push(sym);
                    continue;
                }
                else if (sym == ")")
                {
                    while (cal.top() != "(")
                    {
                        ans.push_back(cal.top());
                        cal.pop();
                    }
                    cal.pop();
                }
                else
                {
                    while (!cal.empty())
                    {
                        std::string top = cal.top();
                        if (comp[sym] > comp[top])
                        {
                            break;
                        }
                        else
                        {
                            ans.push_back(top);
                            cal.pop();
                        }
                    }
                    cal.push(sym);
                }
            }
        }

        while (!cal.empty())
        {
            ans.push_back(cal.top());
            cal.pop();
        }
        return ans;
    }
    int work(std::vector<std::string> &tokens, std::map<std::string, int> &comp)
    {
        std::stack<int> s;
        int num = 0;
        for (auto c : tokens)
        {
            if (c == "=")
            {
                break;
            }
            if (comp[c] != 0) // 是符号
            {
                int ans = 0;
                int ret = comp[c];

                if (comp[c] != 7)
                {
                    // 拿到运算数据
                    int a = s.top();
                    s.pop(); // 先拿到的是右
                    int b = s.top();
                    s.pop(); // 然后是左

                    if (ret == 1)
                    {
                        ans = b || a;
                    }
                    else if (ret == 2)
                    {
                        ans = b && a;
                    }
                    else if (ret == 3)
                    {
                        if (c == "==")
                        {
                            ans = b == a;
                        }
                        else
                        {
                            ans = b != a;
                        }
                    }
                    else if (ret == 4)
                    {
                        if (c == ">")
                        {
                            ans = b > a;
                        }
                        else if (c == ">=")
                        {
                            ans = b >= a;
                        }
                        else if (c == "<")
                        {
                            ans = b < a;
                        }
                        else
                        {
                            ans = b <= a;
                        }
                    }
                    else if (ret == 5)
                    {
                        if (c == "+")
                        {
                            ans = b + a;
                        }
                        else
                        {
                            ans = b - a;
                        }
                    }
                    else if (ret == 6)
                    {
                        if (c == "*")
                        {
                            ans = b * a;
                        }
                        else if (c == "/")
                        {
                            if (a != 0)
                                ans = b / a;
                            err_code_ = ERROR_DIVIDE_BY_ZERO;
                        }
                        else
                        {
                            ans = b % a;
                        }
                    }
                }
                else
                {
                    // 拿到运算数据
                    int a = s.top();
                    s.pop(); // 单目

                    ans = !a;
                }
                s.push(ans); // 结果入栈
            }
            else
            {
                // 是数字 -- 要string转int,然后数字入栈
                if (c[0] == '-')
                {
                    c.erase(0, 1);
                    num = stoi(c);
                    num = -num;
                }
                else
                {
                    num = stoi(c);
                }
                s.push(num);
            }
        }
        return s.top(); // 最后一个元素就是结果
    }

public:
    int err_code_;
};