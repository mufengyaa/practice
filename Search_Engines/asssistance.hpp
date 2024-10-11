#include <iostream>
#include <fstream>
#include <string>

namespace ns_helper
{
    struct doc_info
    {
        std::string title_;
        std::string content_;
        std::string url_;
    };

    void read_file(const std::string &path, std::string *data)
    {
        std::ifstream in(path, std::ios_base::in);
        if (!in.is_open())
        {
            std::cerr << "file: " << path.c_str() << " open failed" << std::endl;
        }
        std::string line;
        while (std::getline(in, line))
        {
            (*data) += line;
        }
        in.close();
    }

};