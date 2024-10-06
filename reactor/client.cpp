#include "client.hpp"
#include <iostream>

using namespace std;

void helper()
{
    cout << "Instructions for use :"
         << "./cal_client ip port" << endl;
}

int main(int argc, char *argv[])
{
    lg.enable();
    if (argc != 3)
    {
        helper();
        exit(0);
    }
    client c(stoi(argv[2]), argv[1]);
    c.run();
    return 0;
}