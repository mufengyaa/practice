#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

namespace helper
{
    // windows
#ifdef _WIN32
#include <windows.h>
    int get_page_shift()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        return static_cast<int>(log2(sysInfo.dwPageSize));
    }
    static const int PAGE_SHIFT = get_page_shift();
    void *allocate_memory(size_t page)
    {
        // 64-bit Windows
        return VirtualAlloc(nullptr, page << PAGE_SHIFT, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    }

    void deallocate_memory(void *ptr)
    {
        VirtualFree(ptr, 0, MEM_RELEASE);
    }
// 判断是32位还是64位的Windows
#if defined(_WIN64)
    using page_t = unsigned long long; // 64位系统
#else
    using page_t = size_t; // 32位系统
#endif

#elif __linux__
// linux
#include <sys/mman.h>
#include <unistd.h>
    static const int PAGE_SHIFT = static_cast<int>(log2(sysconf(_SC_PAGESIZE)));

    void *allocate_memory(size_t page)
    {
        void *ptr = mmap(nullptr, page << PAGE_SHIFT, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr == MAP_FAILED)
        {
            std::cerr << "Memory allocation failed: " << strerror(errno) << std::endl;
            exit(1);
        }
        return ptr;
    }
// 判断是32位还是64位的Linux
#if defined(__x86_64__) || defined(_M_X64)
    using page_t = unsigned long long; // 64位系统
#elif defined(__i386__) || defined(_M_IX86)
    using page_t = size_t; // 32位系统
#else
#error "Unknown architecture"        // 无法识别的架构
#endif

#else
#error "Unsupported platform"
#endif

    static const int PTR_SIZE = sizeof(void *);
    static const int BUCKET_SIZE = 208;
    static int BASIC_BUCKETS_SIZE[5] = {0, 128, 1024, 8 * 1024, 64 * 1024};
    static int MAX_BUCKETS_NUM[5] = {16, 72, 128, 184, 208};
    static const int MAX_PAGE_NUM = 128;

    int feedback_regulation(size_t size)
    {
        int page_size = 1 << PAGE_SHIFT;
        int num = page_size / size;
        if (num < 2)
        {
            num = 2;
        }
        if (num > 512)
        {
            num = 512;
        }
        return num;
    }
    int size_to_page(size_t size)
    {
        int num = feedback_regulation(size); // 要取出的对象个数
        int page_num = (size * num) >> PAGE_SHIFT;
        if (page_num < 1)
        {
            page_num = 1;
        }
        return page_num;
    }
    page_t ptr_to_pageid(void *ptr)
    {
        page_t page_id = ((page_t)ptr) >> PAGE_SHIFT;
        return page_id;
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
        // if (size % align_num != 0)
        // {
        //     return ((size / align_num + 1) * align_num);
        // }
        // return size;
        return ((size + align_num - 1) & ~(align_num - 1));
    }
    int size_to_bucket(size_t size) // 映射到哪个桶
    {
        // size=130
        int i = 0;
        int align_num = alignment(size, i);                     // align_num=16 i=2
        int size_offset = size - BASIC_BUCKETS_SIZE[i - 1] - 1; // size_offset=130-128-1=1
        int bucket_offset = size_offset / align_num;            // bucket_offset=1/16=0
        int bucket_id = bucket_offset;                          // bucket_id=0
        if (i > 1)
        {
            bucket_id += MAX_BUCKETS_NUM[i - 2]; // bucket_id=0+16=16
        }
        return bucket_id;
    }
};
