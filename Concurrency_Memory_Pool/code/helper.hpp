#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <string.h>

static const int PTR_SIZE = sizeof(void *);

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
#elif __linux__
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
#else
#error "Unsupported platform"
#endif