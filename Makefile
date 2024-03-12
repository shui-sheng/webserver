CXX=g++
CXXFLAGS=-g -pthread -lmysqlclient
SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
TEST_DIR=test

OBJ_FILES=$(wildcard $(OBJ_DIR)/*.o)


all: srcbin testbin 

srcbin : obj
	g++  $(SRC_DIR)/main.cpp $(OBJ_DIR)/*.o -o $(BUILD_DIR)/main.bin  $(CXXFLAGS)

testbin : obj
	g++  $(TEST_DIR)/Log_test.cpp $(OBJ_DIR)/*.o		 	-o $(BUILD_DIR)/logtest.bin 		$(CXXFLAGS)
	g++  $(TEST_DIR)/Net_test.cpp $(OBJ_DIR)/*.o		 	-o $(BUILD_DIR)/nettest.bin 		$(CXXFLAGS)
	g++  $(TEST_DIR)/Thread_test.cpp $(OBJ_DIR)/*.o	  		-o $(BUILD_DIR)/threadtest.bin 		$(CXXFLAGS)
	g++  $(TEST_DIR)/ThreadPool_test.cpp $(OBJ_DIR)/*.o  	-o $(BUILD_DIR)/threadpooltest.bin  $(CXXFLAGS)
	g++  $(TEST_DIR)/Time_test.cpp $(OBJ_DIR)/*.o			-o $(BUILD_DIR)/timetest.bin 		$(CXXFLAGS)
	g++  $(TEST_DIR)/Http_test.cpp $(OBJ_DIR)/*.o			-o $(BUILD_DIR)/httptest.bin 		$(CXXFLAGS)


obj : 
	$(MAKE) -C src 	
	
	

clean:
	$(MAKE) -C src clean


