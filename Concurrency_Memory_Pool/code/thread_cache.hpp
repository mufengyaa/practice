#include "helper.hpp"
#include "list.hpp"
#include <unordered_map>

static const int MAX_THREAD = 208;
static int BASIC_SIZE[5] = {0, 128, 1024, 8 * 1024, 64 * 1024};
static int MAX_BUCKET_NUM[5] = {16, 72, 128, 184, 208};

class Thread_Cache
{
    FreeList thread_map_[MAX_THREAD];

public:
    void *alloc(size_t size)
    {
        int bucket_id = size_to_bucket(size);
        // 优先取出桶里的内存
        if (!thread_map_[bucket_id].empty())
        {
            return thread_map_[bucket_id].pop();
        }
        // 如果没有,去向central cache要
    }
    void dealloc(void *ptr, size_t size)
    {
        // 将结点挂回自由链表中
        int bucket_id = size_to_bucket(size);
        thread_map_[bucket_id].push(ptr);
        // 当结点个数达到一定条件,将一批结点归还给central cache
    }

private:
    int size_to_bucket(size_t size) // 映射到哪个桶
    {
        // size=130
        int i = 0;
        int align_num = alignment(size, i);             // align_num=16 i=2
        int size_offset = size - BASIC_SIZE[i - 1] - 1; // size_offset=130-128-1=1
        int bucket_offset = size_offset / align_num;    // bucket_offset=1/16=0
        int bucket_id = bucket_offset;                  // bucket_id=0
        if (i > 1)
        {
            bucket_id += MAX_BUCKET_NUM[i - 2]; // bucket_id=0+16=16
        }
        return bucket_id;
    }
    int alignment(size_t size, int &i) // 以字节为单位
    {
        int align_num = 0;
        assert(size > 0);
        if (size <= 128)
        {
            i = 1;
            align_num = 8;
        }
        else if (size <= 1024)
        {
            i = 2;
            align_num = 16;
        }
        else if (size <= 8 * 1024)
        {
            i = 3;
            align_num = 128;
        }
        else if (size <= 64 * 1024)
        {
            i = 4;
            align_num = 1024;
        }
        else
        {
            i = 5;
            align_num = 8 * 1024;
        }
        return align_num;
    }
    inline size_t Round_up(size_t size, size_t align_num) // 向上取整
    {
        // return ((size / align_num + 1) * align_num);
        return ((size + align_num - 1) & ~(align_num - 1));
    }
};