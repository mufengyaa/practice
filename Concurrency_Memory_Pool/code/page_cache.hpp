#include "helper.hpp"
#include "list.hpp"
#include <unordered_map>

class Page_Cache
{
    static Page_Cache instance_;
    std::unordered_map<helper::page_t, Span *> page_to_span_;
    SpanList span_map_[helper::MAX_PAGE_NUM + 1];

public:
    static std::mutex mtx_;

public:
    static Page_Cache *get_instance()
    {
        return &instance_;
    }
    Span *obj_to_span(void *ptr)
    {
        int page_id = ((helper::page_t)ptr) >> helper::PAGE_SHIFT;
        if (page_to_span_.find(page_id) == page_to_span_.end())
        {
            return nullptr;
        }
        return page_to_span_[page_id];
    }
    Span *alloc(int page)
    {
        Span *span = nullptr;
        // 当前有对应页的span
        if (!span_map_[page].empty())
        {
            span = span_map_[page].pop_front();
        }
        // 继续往下找
        else
        {
            for (int i = page + 1; i <= helper::MAX_PAGE_NUM; ++i)
            {
                if (!span_map_[i].empty())
                {
                    span = span_map_[i].pop_front();
                    break;
                }
            }

            // 找到了就切分成两个span
            if (span != nullptr)
            {
                Span *newspan = new Span;
                // 大span的前page个页属于分配出去的span,剩下的部分链入到对应位置
                newspan->n_ = span->n_ - page;
                newspan->start_pageid_ = span->start_pageid_ + page;
                span_map_[newspan->n_].insert(newspan);

                span->n_ = page;

                // 建立页号->span的地址
                // span是要分配给central cache,最终给thread cache,会涉及到内存回收问题,所以需要每页都映射
                for (int i = 0; i < span->n_; ++i)
                {
                    page_to_span_[span->start_pageid_ + i] = span;
                }
                // newspan留在page cache,只需要被合并,所以首尾页映射即可
                page_to_span_[span->start_pageid_] = newspan;
                page_to_span_[span->start_pageid_ + span->n_ - 1] = newspan;
            }
            // 如果实在找不到span,向系统申请128页
            else
            {
                void *ptr = helper::allocate_memory(helper::MAX_PAGE_NUM);
                // 创建并初始化span
                span = new Span;
                span->n_ = helper::MAX_PAGE_NUM;
                span->start_pageid_ = helper::ptr_to_pageid(ptr);
                span_map_[span->n_].insert(span);
                return alloc(page);
            }
            return span;
        }
    }
    void recycle(Span *span)
    {
        // 合并
        int page_id = span->start_pageid_ - 1;
        while (true) // 往前找
        {
            // 这里需要页号->span的映射关系
            // 找到了就合并
            if (page_to_span_.find(page_id) != page_to_span_.end())
            {
                Span *merged_span = page_to_span_[page_id];
                // 存在但不一定处于空闲状态,该span现在不一定链接在page cache中
                if (merged_span->is_use_ == false)
                {
                    span->n_ += merged_span->n_;
                    span->start_pageid_ -= merged_span->n_;
                    page_id -= merged_span->n_;

                    // 解除[被合并的页]和其他结构的关系,并释放span
                    page_to_span_.erase(page_id);
                    span_map_[merged_span->n_].remove(merged_span);
                    delete merged_span;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        page_id += span->n_;
        while (true) // 往后找
        {
            if (page_to_span_.find(page_id) != page_to_span_.end())
            {
                Span *merged_span = page_to_span_[page_id];
                if (merged_span->is_use_ == false)
                {
                    span->n_ += merged_span->n_;
                    page_id += merged_span->n_;

                    // 解除[被合并的页]和其他结构的关系,并释放span
                    page_to_span_.erase(page_id);
                    span_map_[merged_span->n_].remove(merged_span);
                    delete merged_span;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        span_map_[span->n_].insert(span);
        page_to_span_[span->start_pageid_] = span;
        page_to_span_[span->start_pageid_ + span->n_ - 1] = span;
    }

private:
    Page_Cache() {}
    ~Page_Cache() {}
    Page_Cache(const Page_Cache &) = delete;
    Page_Cache &operator=(const Page_Cache &) = delete;
};

std::mutex Page_Cache::mtx_;
Page_Cache Page_Cache::instance_;
