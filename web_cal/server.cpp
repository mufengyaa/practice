// #include"cal_server.hpp"
#include "cal.hpp"

using namespace std;

int main()
{
    calculate Cal;
    string buffer;
    getline(cin, buffer); // buffer不带回车

    string content = encode(buffer); //"size\nbuffer\n"

    string ret = Cal.cal(content);
    cout << ret;
    return 0;
}