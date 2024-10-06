#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <map>
using namespace std;

void confirm_priority(map<string, int> &comp)
{
    comp.insert(make_pair("=", -1));

    comp.insert(make_pair("||", 1));

    comp.insert(make_pair("&&", 2));

    comp.insert(make_pair("==", 3));
    comp.insert(make_pair("!=", 3));

    comp.insert(make_pair(">", 4));
    comp.insert(make_pair("<", 4));
    comp.insert(make_pair("<=", 4));
    comp.insert(make_pair(">=", 4));

    comp.insert(make_pair("+", 5));
    comp.insert(make_pair("-", 5));

    comp.insert(make_pair("*", 6));
    comp.insert(make_pair("/", 6));
    comp.insert(make_pair("%", 6));

    comp.insert(make_pair("!", 7));
}

vector<string> to_suffix(string arr, map<string, int> &comp)
{
    string numbers("0123456789");
    vector<string> ans;
    stack<string> cal;
    for (auto i = 0; i < arr.size(); ++i)
    {
        if (arr[i] >= '0' && arr[i] <= '9')
        {
            auto pos = arr.find_first_not_of(numbers, i);
            string tmp_arr(arr.begin() + i, arr.begin() + pos);
            ans.push_back(tmp_arr);
            i = pos - 1;
        }
        else
        { // 符号
            string sym;
            sym += arr[i];
            if (arr[i + 1] < '0' || arr[i + 1] > '9')
            {
                ++i;
                sym += arr[i];
            }

            if (sym == "(")
            {
                cal.push(sym);
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
                    string top = cal.top();
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

int evalRPN(vector<string> &tokens, map<string, int> &comp)
{
    std::stack<int> s;
    int num = 0;
    for (auto c : tokens)
    {
        int flag = 1;
        if (comp[c] != 0) // 是符号
        {
            int ans = 0;
            size_t i = 0;
            int ret = comp[c];

            if (comp[c] != 7)
            {
                // 拿到运算数据
                int a = s.top();
                s.pop(); // 先拿到的是右
                int b = s.top();
                s.pop(); // 然后是左

                if (ret == -1)
                {
                    break;
                }
                else if (ret == 1)
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
                        ans = b / a;
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
            num = stoi(c);
            s.push(num);
        }
    }
    return s.top(); // 最后一个元素就是结果
}

int cal()
{
    map<string, int> comp;
    confirm_priority(comp);
    string arr;
    cin >> arr;
    vector<string> ret = to_suffix(arr, comp);
    int ans = evalRPN(ret, comp);
    return ans;
}

int main()
{
    cal();
    return 0;
}