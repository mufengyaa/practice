#include "thread.hpp"
#include <time.h>

using namespace std;

void test1(int x)
{
    cout << x << endl;
    usleep(20);
}
void test2(string data, int id)
{
    usleep(20);
    cout << id << ":data is " << data << endl;
    usleep(20);
}
int main()
{
    thread<int> t1(test1, 100);
    thread<string, int> t2(test2, "hello world", 2);
    t1.join();
    t2.join();
    cout << t1.get_name() << endl;
    cout << t2.get_name() << ":" << t2.get_time() << endl;
    return 0;
}