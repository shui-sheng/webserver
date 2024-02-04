CXX=g++
CXXFLAGS=-g -pthread
SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
TEST_DIR=test

OBJ_FILES=$(wildcard $(OBJ_DIR)/*.o)


all: srcbin testbin 

srcbin : obj
	g++ $(CXXFLAGS) $(SRC_DIR)/main.cpp $(OBJ_DIR)/*.o				-o $(BUILD_DIR)/main.bin

testbin : obj
	g++ $(CXXFLAGS) $(TEST_DIR)/Log_test.cpp $(OBJ_DIR)/*.o		 	-o $(BUILD_DIR)/logtest.bin
	g++ $(CXXFLAGS) $(TEST_DIR)/Net_test.cpp $(OBJ_DIR)/*.o		 	-o $(BUILD_DIR)/nettest.bin
	g++ $(CXXFLAGS) $(TEST_DIR)/Thread_test.cpp $(OBJ_DIR)/*.o	  	-o $(BUILD_DIR)/threadtest.bin
	g++ $(CXXFLAGS) $(TEST_DIR)/ThreadPool_test.cpp $(OBJ_DIR)/*.o  -o $(BUILD_DIR)/threadpooltest.bin
	g++ $(CXXFLAGS) $(TEST_DIR)/Time_test.cpp $(OBJ_DIR)/*.o		-o $(BUILD_DIR)/timetest.bin
	g++ $(CXXFLAGS) $(TEST_DIR)/Http_test.cpp $(OBJ_DIR)/*.o		-o $(BUILD_DIR)/httptest.bin


obj : 
	$(MAKE) -C src 	
	
	

clean:
	$(MAKE) -C src clean
