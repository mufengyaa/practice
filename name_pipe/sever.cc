#include "comm.hpp"
#include"log.hpp"

// 用于接收信息(服务端)

void recive_message(int fd){
    char buffer[num];
    while (true)
    {
        memset(buffer, 0, sizeof buffer);
        ssize_t size = read(fd, buffer, sizeof buffer - 1);
        //cout<<size<<endl;
        if (size < 0)
        {
            perror("read");
            exit(2);
        }
        else if (size == 0)
        {
            cerr <<"[ " << getpid() << " ]" << "read end , client quit , sever quit too " << endl;
            break;
        }
        else
        {
            cout << "[ " << getpid() << " ]" <<" send_message is : " << buffer << endl;
            log("读取信息成功",debug);
        }
    }
}
int main()
{
    // 创建管道文件
    if (mkfifo(pipe_path.c_str(), mode) < 0)
    {
        perror("mkfifo");
    }
    log("创建管道文件成功",debug);
    // 文件操作
    int fd = open(pipe_path.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }
    log("打开文件成功",debug);
    for(int i=0;i<process_size;i++){
      size_t id=fork();
      if(id==0){
        recive_message(fd);
        exit(0);
      }
    }
    for(int i=0;i<process_size;i++){
      pid_t ret=waitpid(-1,nullptr,0);
    }
    // 关闭
    close(fd);
    log("关闭管道文件成功",debug);
    unlink(pipe_path.c_str());
    log("删除管道文件成功",debug);
    return 0;
}
