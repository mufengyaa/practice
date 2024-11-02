#include "helper.hpp"
#include "list.hpp"
#include "central_cache.hpp"

class Thread_Cache
{
    FreeList thread_map_[helper::BUCKET_SIZE];

public:
    void *alloc(size_t size)
    {
        int bucket_id = helper::size_to_bucket(size);

        // 优先取出桶里的内存
        if (!thread_map_[bucket_id].empty())
        {
            --thread_map_[bucket_id].size();
            //  std::cout << "从thread cache拿出1个对象," << thread_map_[bucket_id].size() << std::endl;
            return thread_map_[bucket_id].pop();
        }

        // 如果没有,去向central cache要
        int i = 0;                                                                                 // 无用
        return alloc_central_cache(bucket_id, helper::Round_up(size, helper::alignment(size, i))); // 传入对齐后的对象大小
    }

    void dealloc(void *ptr, size_t size)
    {
        // 将结点挂回自由链表中
        int bucket_id = helper::size_to_bucket(size);
        thread_map_[bucket_id].push_front(ptr);
        ++thread_map_[bucket_id].size();
        // std::cout << "还给thread cache1个对象," << thread_map_[bucket_id].size() << std::endl;

        // 当结点个数达到一定条件,将一批结点归还给central cache
        if (thread_map_[bucket_id].size() > thread_map_[bucket_id].benchmark() + 1)
        {
            int i = 0;
            //  std::cout << "回收给central cache\n";
            Central_Cache::get_instance()->recycle(thread_map_[bucket_id], helper::alignment(size, i));
            thread_map_[bucket_id].size() -= thread_map_[bucket_id].size();
        }
    }

private:
    void *alloc_central_cache(int bucket_id, size_t size)
    {
        void *start = nullptr, *end = nullptr;
        int num = Central_Cache::get_instance()->alloc(start, end, bucket_id, size, thread_map_[bucket_id].benchmark()); // 从central cache中拿出一串内存
        if (num > 1)
        {
            void *node = thread_map_[bucket_id].next(start);
            void *next = node;
            while (node != nullptr && node != end)
            {
                next = thread_map_[bucket_id].next(node);
                thread_map_[bucket_id].push_back(node);
                node = next;
                ++thread_map_[bucket_id].size();
            }
        }
        return start;
    }
};

// windows
#ifdef _WIN32
static _declspec(thread) Thread_Cache *TLS_ThreadCache = nullptr;

#else
static __thread Thread_Cache *TLS_ThreadCache = nullptr;
#endif
