#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>

#include "../build/inc/cppjieba/Jieba.hpp"
#include "../build/inc/cppjieba/limonp/StringUtil.hpp"
#include "../build/inc/cppjieba/limonp/Logging.hpp"

#define source_file_path "../input/"
#define target_path "../data/data.txt"
#define delimiter '\3'

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

    const char *const DICT_PATH = "/home/mufeng/c++/Search_Engines/build/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "/home/mufeng/c++/Search_Engines/build/dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "/home/mufeng/c++/Search_Engines/build/dict/user.dict.utf8";
    const char *const IDF_PATH = "/home/mufeng/c++/Search_Engines/build/dict/idf.utf8";
    const char *const STOP_WORD_PATH = "/home/mufeng/c++/Search_Engines/build/dict/stop_words.utf8";
    class JiebaUtil
    {
    private:
        cppjieba::Jieba jieba;
        std::unordered_map<std::string, bool> stop_words;

    private:
        JiebaUtil() : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
        {
            std::cout << "init success" << std::endl;
        }
        JiebaUtil(const JiebaUtil &) = delete;

        static JiebaUtil *instance;

    public:
        static JiebaUtil *get_instance()
        {
            static std::mutex mtx;
            if (nullptr == instance) //
            {
                mtx.lock();
                if (nullptr == instance) // 防止多线程下重复创建对象
                {
                    instance = new JiebaUtil();
                    std::cout << "JiebaUtil success" << std::endl;
                }
                mtx.unlock();
            }

            return instance;
        }

        void CutStringHelper(const std::string &src, std::vector<std::string> *out)
        {
            jieba.CutForSearch(src, *out);
        }

    public:
        static void CutString(const std::string &src, std::vector<std::string> *out)
        {
            ns_helper::JiebaUtil::get_instance()->CutStringHelper(src, out);
        }
    };

    JiebaUtil *JiebaUtil::instance = nullptr;
};