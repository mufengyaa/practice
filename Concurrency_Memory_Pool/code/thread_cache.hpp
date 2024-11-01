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
        int i = 0;                         // 无用
        size = helper::alignment(size, i); // 计算出对齐后的对象大小

        // 如果没有,去向central cache要
        void *start = nullptr, *end = nullptr;
        int num = Central_Cache::get_instance()->alloc(start, end, bucket_id, size, thread_map_[bucket_id].benchmark()); // 从central cache中拿出一串内存
        thread_map_[bucket_id].size() += num;
        //   std::cout << "分配给thread cache" << num << "个对象," << thread_map_[bucket_id].size() << std::endl;

        void *res = start;
        end += size;

        // 从第二个结点开始,链入到链表中
        start = thread_map_[bucket_id].next(start);
        void *next = start;
        // 尾插
        while (start != nullptr && start != end)
        {
            next = thread_map_[bucket_id].next(start);
            thread_map_[bucket_id].push_back(start);
            start = next;
        }
        --thread_map_[bucket_id].size();
        //  std::cout << "从thread cache拿出1个对象," << thread_map_[bucket_id].size() << std::endl;
        return res;
    }
    void dealloc(void *ptr, size_t size)
    {
        // 将结点挂回自由链表中
        int bucket_id = helper::size_to_bucket(size);
        thread_map_[bucket_id].push_front(ptr);
        ++thread_map_[bucket_id].size();
        // std::cout << "还给thread cache1个对象," << thread_map_[bucket_id].size() << std::endl;
        int i = 0;
        size = helper::alignment(size, i);

        // 当结点个数达到一定条件,将一批结点归还给central cache
        if (thread_map_[bucket_id].size() > thread_map_[bucket_id].benchmark() + 1)
        {
            //  std::cout << "回收给central cache\n";
            Central_Cache::get_instance()->recycle(thread_map_[bucket_id], size);
            thread_map_[bucket_id].size() -= thread_map_[bucket_id].size();
        }
    }
};

static __thread Thread_Cache *TLS_ThreadCache = nullptr;