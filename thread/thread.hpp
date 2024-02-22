#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <functional>
#include <iostream>

template <typename... Args>
using callback_t = std::function<void(Args...)>; // 定义一个函数类型,用于接收传入的函数指针

static int NUM = 1;

template <class... Args>
class thread
{
public:
    thread(callback_t<Args...> cb, Args... args)
        : tid_(0), name_(""), start_timestamp_(0), args_(std::make_tuple(args...)), callback_(cb) // 将参数包展开，创建一个元组
    {
        std::string name = "pthread" + std::to_string(NUM++);
        name_ = name;
        start_timestamp_ = time(nullptr);

        pthread_create(&tid_, nullptr, entry, this);
    }
    ~thread()
    {
    }
    void join()
    {
        pthread_join(tid_, nullptr);
    }
    std::string get_name()
    {
        return name_;
    }
    uint64_t get_time()
    {
        return start_timestamp_;
    }
    pthread_t get_id()
    {
        return tid_;
    }

private:
    // 使用索引序列传递
    static void *entry(void *args) // 调用存放的函数方法
    {
        thread *it = static_cast<thread *>(args);
        it->call(std::index_sequence_for<Args...>()); // 根据参数包生成索引序列
        return nullptr;
    }

    template <std::size_t... Is>
    void call(std::index_sequence<Is...>)
    {
        callback_(std::get<Is>(args_)...); // 将元组中的参数按照索引序列,逐个传递给callback_
    }

    // 使用折叠表达式直接展开
    static void *entry(void *args)
    {
        thread *it = static_cast<thread *>(args);
        std::apply(it->callback_, it->args_);
        return nullptr;
    }

private:
    pthread_t tid_;
    callback_t<Args...> callback_; // 存储函数方法
    std::tuple<Args...> args_;     // 存储参数包

    std::string name_;
    uint64_t start_timestamp_;
};
