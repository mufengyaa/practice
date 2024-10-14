#include "searcher.hpp"

int main()
{
    Searcher s;
    std::string json;
    s.search("bracket", &json);
    std::cout << json << std::endl;
    return 0;
}