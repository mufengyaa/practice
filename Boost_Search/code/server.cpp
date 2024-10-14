#include "searcher.hpp"
#include "/home/mufeng/cpp-httplib/httplib.h"
#include<pthread.h>

#define root_path "../wwwroot"

int main()
{
    Searcher s;

    httplib::Server svr;
    svr.set_base_dir(root_path);
    svr.Get("/s", [&s](const httplib::Request &req, httplib::Response &rsp)
            {
            if(!req.has_param("word")){
                rsp.set_content("必须要有搜索关键字!", "text/plain; charset=utf-8");
                return;
            }
            std::string word = req.get_param_value("word");

            std::cout << "用户在搜索：" << word << std::endl;

            std::string json_string;
            s.search(word,&json_string);
            rsp.set_content(json_string, "application/json"); });

    svr.listen("0.0.0.0", 8080);
    return 0;
}