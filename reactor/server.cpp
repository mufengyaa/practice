#include "server.hpp"
#include "cal.hpp"

void def_handle(std::weak_ptr<connection> conne)
{
    if (conne.expired())
        return;
    auto conn = conne.lock();

    calculate Cal;
    std::string str = Cal.cal(conn->inbuffer()); // 处理数据,得到结果
    if (str.empty())
    {
        return;
    }
    //lg(DEBUG, "get data: %s\n", str.c_str());

    conn->out_append(str); // 添加到输出缓冲区
    //lg(DEBUG, "out_append success\n");

    // 写入
    auto server = conn->p_svr_.lock(); // weak_ptr不拥有对象的所有权,需要转换为shared_ptr
    server->sender(conn);             // 需要让服务器调用写处理函数,后续让服务器擦屁股(也许没有写入全部数据)
    //lg(DEBUG, "sender success\n");
}

int main()
{
    std::shared_ptr<epoll_server> epoll_svr(new epoll_server(8080, def_handle));
    epoll_svr->loop();
    return 0;
}