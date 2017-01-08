CC=gcc
CCP=g++
AR=ar -rcs
CFLAGS=-c -Wall -Werror
GTEST_DIR=dependencies/googletest
GLFW_DIR=dependencies/glfw
GLAD_DIR=dependencies/glad
CPPFLAGS=-c -Wall -Werror -std=c++11
TEST_INCLUDE_PATHS=-Isrc/glsl_algo/ -I$(GTEST_DIR)/googletest/include -I$(GLFW_DIR)/include -I$(GLAD_DIR)/out/include -I$(COMMON_DIR)
GLSL_ALGO_DIR=src/glsl_algo
TEST_DIR=src/tests
COMMON_DIR=src/common
OUTPUT_DIR=out
MKDIR_C=mkdir -p

.PHONY: clean create_out_dir configure

all: build test

create_out_dir:
	$(MKDIR_C) out

configure:
	python src/configurator/basic_configurator.py
	
build: configure create_out_dir libglslalgo.a

#library
libglslalgo.a: prefix_scan.o radix_sort.o
	$(AR) $(OUTPUT_DIR)/libglslalgo.a $(OUTPUT_DIR)/prefix_scan.o $(OUTPUT_DIR)/radix_sort.o
	
prefix_scan.o: $(GLSL_ALGO_DIR)/prefix_scan.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/prefix_scan.c -o $(OUTPUT_DIR)/prefix_scan.o
	
radix_sort.o: $(GLSL_ALGO_DIR)/radix_sort.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/radix_sort.c -o $(OUTPUT_DIR)/radix_sort.o

#tests
test: build gtest glfw glad test_main.o prefix_scan_tests.o util_tests.o gl_setup.o
	$(CCP) -o $(OUTPUT_DIR)/test_main $(OUTPUT_DIR)/test_main.o $(OUTPUT_DIR)/prefix_scan_tests.o $(OUTPUT_DIR)/util_tests.o $(OUTPUT_DIR)/gl_setup.o -L$(OUTPUT_DIR) -lglslalgo -lgtest -pthread -lglfw3 -lX11 -ldl -lXrandr -lXi -lXinerama -lXcursor -lGL -lglad

test_main.o: $(TEST_DIR)/main.cpp
	$(CCP) $(CPPFLAGS) $(TEST_INCLUDE_PATHS) $(TEST_DIR)/main.cpp -o $(OUTPUT_DIR)/test_main.o

prefix_scan_tests.o: $(TEST_DIR)/prefix_scan_tests.cpp
	$(CCP) $(CPPFLAGS) $(TEST_INCLUDE_PATHS) $(TEST_DIR)/prefix_scan_tests.cpp -o $(OUTPUT_DIR)/prefix_scan_tests.o

util_tests.o: $(TEST_DIR)/util_tests.cpp
	$(CCP) $(CPPFLAGS) $(TEST_INCLUDE_PATHS) $(TEST_DIR)/util_tests.cpp -o $(OUTPUT_DIR)/util_tests.o

gtest:
	cd $(GTEST_DIR)/googletest/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GTEST_DIR)/googletest/out/libgtest.a $(OUTPUT_DIR)/libgtest.a

#common
gl_setup.o: $(COMMON_DIR)/gl_setup.cpp
	$(CCP) $(CPPFLAGS) -I$(GLFW_DIR)/include -I$(GLAD_DIR)/out/include $(COMMON_DIR)/gl_setup.cpp -o $(OUTPUT_DIR)/gl_setup.o

glfw:
	cd $(GLFW_DIR)/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GLFW_DIR)/out/src/libglfw3.a $(OUTPUT_DIR)/libglfw3.a

glad:
	cd $(GLAD_DIR)/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GLAD_DIR)/out/libglad.a $(OUTPUT_DIR)/libglad.a

clean:
		rm -rf $(OUTPUT_DIR)/
		rm -rf $(GTEST_DIR)/googletest/out
		rm -rf $(GLFW_DIR)/out
		rm -rf $(GLAD_DIR)/out