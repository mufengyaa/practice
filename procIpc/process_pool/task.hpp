#include <iostream>
#include <unistd.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

using namespace std;

using func = function<void()>;
vector<func> callbacks;
unordered_map<int, string> descripe;

void read_mysql()
{
    cout << "process: " << getpid() << " 执行访问数据库任务" << endl;
}

void execule_url()
{
    cout << "process: " << getpid() << " 执行url解析" << endl;
}

void cal()
{
    cout << "process: " << getpid() << " 执行加密任务" << endl;
}

void save()
{
    cout << "process: " << getpid() << " 执行访问数据持久化任务" << endl;
}

void load()
{
    descripe.insert({callbacks.size(), "read_mysql"});
    callbacks.push_back(read_mysql);
    for (const auto &i : descripe)
    {
        cout << i.first << " : " << i.second << endl;
    }

    descripe.insert({callbacks.size(), "execule_url"});
    callbacks.push_back(execule_url);

    descripe.insert({callbacks.size(), "cal"});
    callbacks.push_back(cal);

    descripe.insert({callbacks.size(), "save"});
    callbacks.push_back(save);
}

void show_handler()
{
    //cout<<"im show "<<endl;
    for (const auto &i : descripe)
    {
        cout << i.first << " : " << i.second << endl;
    }
}

int task_size(){
    return callbacks.size();
}