// 建立正排和倒排索引,并提供查找接口
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <boost/filesystem.hpp>

#include "asssistance.hpp"

#define source_file_path "../input"
#define target_path "../data/data.txt"
#define delimiter '\3'

namespace fs = boost::filesystem;

struct docinfo_index : public ns_helper::doc_info
{
};

class Index
{
public:
    Index() {}
    ~Index() {}

private:
    void create_positive_index() {}
    void create_inverted_index() {}
};