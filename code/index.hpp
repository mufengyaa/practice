// 建立正排和倒排索引,并提供查找接口
#pragma once

#include <utility>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "assistance.hpp"

#define doc_id_t long

namespace fs = boost::filesystem;

struct docInfo_index : public ns_helper::doc_info
{
    doc_id_t doc_id_;
};
struct word_info
{
    std::string word_;
    doc_id_t doc_id_;
    int weight_; // 这个词在文档中的权重
};
using inverted_zipper = std::vector<word_info>;

class Index
{
    std::vector<docInfo_index> pos_index_;                       // 正排索引
    std::unordered_map<std::string, inverted_zipper> inv_index_; // 倒排索引

public:
    Index() {}
    ~Index() {}
    void init(const std::string &path)
    {
        create_positive_index(path);
        std::cout<<"create_positive_index success"<<std::endl;
        for (const auto &it : pos_index_)
        {
            create_inverted_index(it);
        }
        std::cout<<"create_inverted_index success"<<std::endl;

    }
    bool search_positive_index(const doc_id_t id, docInfo_index *doc)
    {
        if (id >= pos_index_.size())
        {
            return false;
        }
        doc = &pos_index_[id];
        return true;
    }
    bool search_inverted_index(const std::string &target, inverted_zipper *iz)
    {
        auto ret = inv_index_.find(target);
        if (ret == inv_index_.end())
        {
            return "None";
        }
        else
        {
            iz = &(ret->second);
        }
        return true;
    }

private:
    void create_positive_index(const std::string &path) // 以文档为单位
    {
        std::ifstream in(path, std::ios_base::in);
        if (!in.is_open())
        {
            std::cerr << "file: " << path.c_str() << " open failed" << std::endl;
            return;
        }

        std::string doc;
        while (std::getline(in, doc))
        {
            // 拿到一个文档,进行解析
            docInfo_index di;
            if (!analysis(doc, delimiter, &di))
            {
                std::cerr << "analysis faild" << std::endl;
                continue;
            }
            di.doc_id_ = pos_index_.size();
            // 解析完成后,插入到索引中
            pos_index_.push_back(std::move(di));
        }
    }
    void create_inverted_index(const docInfo_index &doc) // 以文档为单位
    {
        struct word_cnt
        {
            int title_cnt_;
            int content_cnt_;
            word_cnt() : title_cnt_(0), content_cnt_(0) {}
            ~word_cnt() {}
        };
        std::unordered_map<std::string, word_cnt> cnt_map;

        // 统计每个词在所属文档中的相关性
        std::vector<std::string> content_words;
        ns_helper::JiebaUtil::CutString(doc.content_, &content_words);
        for (auto it : content_words)
        {
            // 为了实现匹配时忽略大小写,将所有单词转换为小写
            boost::to_lower(it);
            ++cnt_map[it].content_cnt_;
        }

        std::vector<std::string> title_words;
        ns_helper::JiebaUtil::CutString(doc.title_, &title_words);
        for (auto it : title_words)
        {
            boost::to_lower(it);
            ++cnt_map[it].title_cnt_;
        }

// 计算权值
#define title_count 10
#define content_count 1
        for (const auto &it : cnt_map)
        {
            word_info t;
            t.doc_id_ = doc.doc_id_;
            t.word_ = it.first;
            t.weight_ = (it.second).title_cnt_ * title_count + (it.second).content_cnt_ * content_count;
            inv_index_[t.word_].push_back(std::move(t)); // 插入的是小写单词
        }
    }

    bool analysis(const std::string doc, char sep, docInfo_index *doc_info)
    {
        std::vector<std::string> result;
        boost::split(result, doc, [sep](char c)
                     { return c == sep; }, boost::token_compress_on);
        if (result.size() != 3)
        {
            return false;
        }
        doc_info->title_ = result[0];
        doc_info->content_ = result[1];
        doc_info->url_ = result[2];
        return true;
    }
};