cmake_minimum_required(VERSION 3.0)

# 设置项目名称为DebugMode
project(DebugMode)

# 设置编译选项为-g，即生成调试信息
set(CMACKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 查找所有的.cpp文件
file(GLOB SOURCES *.cpp)

# 将所有的.cpp文件添加到SOURCES变量中
set(SOURCES ${SOURCES} )

# 指定可执行文件的名称为DebugMode，并将SOURCES变量中的所有文件编译链接成可执行文件
add_executable(DebugMode ${SOURCES})

# 指定链接库为pthread
target_link_libraries(DebugMode pthread)

# 设置构建类型为Debug
set(CMAKE_BUILD_TYPE Debug)
