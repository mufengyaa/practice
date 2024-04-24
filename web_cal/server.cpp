#include <limits>
#include <stdexcept>

#include "cal_server.hpp"
#include "cal.hpp"
using namespace std;

void helper()
{
    cout << "Instructions for use :"
         << "./cal_server port" << endl;
}
void test()
{
    calculate Cal;
    string buffer = "1+1=";
    encode(buffer); //"size\nbuffer\n"
    buffer += buffer;
    buffer += buffer;
    buffer += buffer;
    buffer += buffer;

    string ret = Cal.cal(buffer);
    cout << ret;
}
// int main(){
//     test();
//     return 0;
// }

void test1(int port)
{
    calculate Cal;
    server s(port, "0,0,0,0", std::bind(&calculate::cal, &Cal, std::placeholders::_1)); // 使用回调
    s.run();
}
int main(int argc, char *argv[])
{
    // ./cal_server ports

    lg.enable();
    if (argc != 2)
    {
        helper();
        exit(0);
    }

    test1(stoi(argv[1]));
    return 0;
}