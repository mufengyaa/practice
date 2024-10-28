cmake_minimum_required(VERSION 3.10)  # 指定 CMake 的最低版本
project(Concurrency_Memory_Pool VERSION 1.0)         # 定义项目名称和版本

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# 添加可执行文件
add_executable(test_Fixed_length test_Fixed_length.cpp)

target_link_libraries(test_Fixed_length) 

#cmake -DCMAKE_BUILD_TYPE=Release ../code