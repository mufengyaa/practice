#include <fstream>
#include <unordered_map>

#define file_name "./word.txt"
#define delimiter ":"

void segmentation(std::string &info, std::string &part1, std::string &part2)
{
    auto it = info.find(delimiter);
    part1 = info.substr(0, it); // 左闭右开
    part2 = info.substr(it + 1, info.size());
}
struct words_init
{
    words_init()
    {
        std::ifstream in(file_name);
        std::string info;
        while (std::getline(in, info))
        {
            std::string part1, part2;
            segmentation(info, part1, part2);
            words_[part1] = part2;
        }
    }
    std::string translation(const std::string &info)
    {
        auto it = words_.find(info);
        std::string res;
        if (it == words_.end())
        {
            return "UnKnown";
        }
        else
        {
            return it->second;
        }
        return nullptr;
    }

    std::unordered_map<std::string, std::string> words_;
};