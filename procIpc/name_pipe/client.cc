#include"comm.hpp"

//用于发送信息(客户端)
int main(){
    //文件操作
    int fd=open(pipe_path.c_str(),O_WRONLY|O_TRUNC);
    if(fd<0){
        perror("open");
        exit(1);
    }
    string buffer;
    while(true){
        cout<<"Please enter the information you want to send : "<<endl;
        getline(cin,buffer);  //输入要发送的信息
        int size=write(fd,buffer.c_str(),buffer.size());
        if(size<0){
            perror("write");
            exit(2);
        }
        else if(size==0){
            break;
        }
    } 
    exit(fd);
    return 0;
}
