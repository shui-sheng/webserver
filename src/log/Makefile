CXX := g++
CXXFLAGS := -g -pthread -w -fno-inline -lmysqlclient
BUILD_DIR := ../../build
OBJ_DIR := $(BUILD_DIR)/obj

# 源文件列表
SRC_FILES := $(wildcard ./*.cpp)
SRC_HEADERS := $(wildcard ../../include/log/*.h)

# 生成目标文件列表
OBJ_FILES := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES)) 


# 默认目标
all: $(OBJ_FILES) 

# 生成目标文件规则
$(OBJ_DIR)/%.o: ./%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

# 清理规则
clean:
	rm -rf $(OBJ_DIR)
