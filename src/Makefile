CXX=g++
CXXFLAGS=-g -pthread -fno-inline
BUILD_DIR := ../build
OBJ_DIR := $(BUILD_DIR)/obj
SUBDIRS = log net thread time
TARGET = main.cpp


# SRC_FILES := $(wildcard ./*.cpp)

# OBJ_FILES := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# 默认目标
all: submk
	

submk : 
	$(MAKE) -C log
	$(MAKE) -C net
	$(MAKE) -C thread
	$(MAKE) -C time


# # 构建主目标
# $(OBJ_DIR)/%.o: ./%.cpp
# 	mkdir -p $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则
clean :
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir clean; \
    done

