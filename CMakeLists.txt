cmake_minimum_required(VERSION 3.22)
project(Ticket_System)

# 设置 C++ 标准为 14 并且是必须要求的
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含当前目录，以便找到头文件
include_directories(.)

# 为 release 版本添加优化标志，调试版本保留默认设置
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# 将我们的目标（可执行文件）及其相关的源文件和头文件添加到项目中
add_executable(code
        Database/BPT.hpp
        Database/exceptions.hpp
        Database/MemoryRiver.hpp
        main.cpp
)