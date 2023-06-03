cmake_minimum_required(VERSION 3.5)

# 设置项目名称
project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 添加头文件路径
include_directories(include)

# 添加源文件路径
aux_source_directory(src DIR_SRCS )

# 指定生成可执行文件的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 生成可执行文件
add_executable(MyProject ${DIR_SRCS})

###############################################################
# 指定CMake的最低版本要求
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
