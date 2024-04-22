#pragma once

#include <map>
#include <utility>
#include <vector>
#include <stack>
#include <iostream>

#include "Serialization.hpp"
#include "help.hpp"

// 提供计算和解析封装报文的功能

class calculate
{
public:
    calculate()
        : err_code_(0) {}
    ~calculate() {}

    std::string cal(std::string &content) // 封装后的报文,//"size"\n"a op b"\n
    {
        err_code_ = 0;
        // 解析出表达式
        std::string data;
        bool ret = decode(content, data); // data:"a op b"
        if (!ret)                         // 不是完整的报文
        {
            return "";
        }
        // 因为这里我们直接用表达式计算,所以不需要反序列化

        // 计算
        std::map<std::string, int> comp;
        confirm_priority(comp);

        std::vector<std::string> tmp = to_suffix(data, comp);
        if (tmp.empty())
        {
            return "";
        }

        response res;
        res.result_ = work(tmp, comp);
        res.err_code_ = err_code_;

        std::string res_tmp;
        res.serialize(res_tmp); //"result code"
        encode(res_tmp);        // size\n"result code"\n

        return res_tmp;
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
        bool prev_was_operator = true;
        // std::cout << "arr:" << arr << std::endl;
        std::string numbers("0123456789");
        std::vector<std::string> ans, err;
        std::stack<std::string> cal;
        for (auto i = 0; i < arr.size(); ++i)
        {
            // std::cout << "i:" << i << std::endl;
            if (arr[i] >= '0' && arr[i] <= '9')
            {
                auto pos = arr.find_first_not_of(numbers, i);
                if (pos == std::string::npos)
                {
                    // 没有=
                    err_code_ = ERROR_SYNTAX_ERROR;
                    return err;
                }
                std::string tmp_arr(arr.begin() + i, arr.begin() + pos);
                ans.push_back(tmp_arr);
                i = pos - 1;
                prev_was_operator = false;
            }
            else
            {                                                       // 符号
                if (arr[i] == '-' && (prev_was_operator || i == 0)) // Check if it's a negative number or subtraction operator
                {
                    std::string tmp = "-";
                    // Check if next character is a digit
                    if (i + 1 < arr.size() && arr[i + 1] >= '0' && arr[i + 1] <= '9')
                    {
                        auto pos = arr.find_first_not_of(numbers, i + 1);
                        std::string tmp_arr(arr.begin() + i + 1, arr.begin() + pos);
                        ans.push_back(tmp + tmp_arr);
                        i = pos - 1;
                        prev_was_operator = false; // Since we found a negative number, set the flag to false
                    }
                    else // It's a subtraction operator
                    {
                        cal.push(tmp);
                        prev_was_operator = true; // Set the flag to true since it's an operator
                    }
                    continue;
                }
                std::string sym;
                sym += arr[i];
                // std::cout << "sym:" << sym << std::endl;
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
                    if (comp[sym] == 0)
                    {
                        err_code_ = ERROR_INVALID_EXPRESSION;
                        return err;
                    }
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
            // for (auto c : ans)
            // {
            //     std::cout << c << " ";
            // }
            // std::cout << std::endl;
            // if (!cal.empty())
            // {
            //     std::cout << cal.top() << std::endl;
            // }
        }

        while (!cal.empty())
        {
            ans.push_back(cal.top());
            cal.pop();
        }
        // std::cout << "ans: ";
        // for (auto c : ans)
        // {
        //     std::cout << c << " ";
        // }
        // std::cout << std::endl;
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
                            if (a == 0)
                            {
                                err_code_ = ERROR_DIVIDE_BY_ZERO;
                                return 0;
                            }
                            ans = b / a;
                        }
                        else
                        {
                            if (a == 0)
                            {
                                err_code_ = ERROR_DIVIDE_BY_ZERO;
                                return 0;
                            }
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
            else // 数字或其他符号
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
        if (s.empty())
        {
            err_code_ = ERROR_SYNTAX_ERROR;
            return 0;
        }
        return s.top(); // 最后一个元素就是结果
    }

public:
    int err_code_;
};