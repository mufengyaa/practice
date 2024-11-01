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
            return thread_map_[bucket_id].pop();
        }
        int i = 0;                         // 无用
        size = helper::alignment(size, i); // 计算出对齐后的对象大小

        // 如果没有,去向central cache要
        void *start = nullptr, *end = nullptr;
        int num = Central_Cache::get_instance()->alloc(start, end, bucket_id, size); // 从central cache中拿出一串内存
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
        return res;
    }
    void dealloc(void *ptr, size_t size)
    {
        // 将结点挂回自由链表中
        int bucket_id = helper::size_to_bucket(size);
        thread_map_[bucket_id].push_front(ptr);

        // 当结点个数达到一定条件,将一批结点归还给central cache
        if (thread_map_[bucket_id].size() == thread_map_[bucket_id].benchmark())
        {
            void *start = thread_map_[bucket_id].head(), *end = start + thread_map_[bucket_id].size() * size;
            Central_Cache::get_instance()->recycle((char *)start, (char *)end, size);
        }
    }
};