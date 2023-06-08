
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2
LDFLAGS := -lpthread -lhiredis -lmysqlclient

# 获取当前文件夹下的所有 .cpp 文件
CPP_FILES := $(wildcard *.cpp)
# 获取当前文件夹下的所有 .cc 文件
CC_FILES := $(wildcard *.cc)

# 将 .cpp 文件转换为对应的目标文件名
OBJ_FILES := $(CPP_FILES:.cpp=.o)
# 将 .cc 文件转换为对应的目标文件名
OBJ_FILES += $(CC_FILES:.cc=.o)

# 默认目标为可执行文件 main
all: main

# 生成可执行文件 main
main: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 生成 .o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理生成的目标文件和可执行文件
clean:
	rm -f $(OBJ_FILES) main

