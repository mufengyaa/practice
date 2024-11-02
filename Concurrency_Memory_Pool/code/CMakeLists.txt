cmake_minimum_required(VERSION 3.10)  # 指定 CMake 的最低版本
project(Concurrency_Memory_Pool VERSION 1.0)  # 定义项目名称和版本

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译优化选项
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")  # 如果需要调试信息可以取消注释

# 添加可执行文件
add_executable(test_alloc test_alloc.cpp)

# 链接 pthread 和 gperftools 库
target_link_libraries(test_alloc pthread)


#cmake -DCMAKE_BUILD_TYPE=Release ../code