CC=gcc
CCP=g++
AR=ar -rcs
CFLAGS=-c -Wall -Werror
GTEST_DIR=dependencies/googletest
GBENCHMARK_DIR=dependencies/benchmark
GLFW_DIR=dependencies/glfw
GLAD_DIR=dependencies/glad
CPPFLAGS=-c -Wall -Werror -std=c++11
TEST_INCLUDE_PATHS=-Isrc/ -I$(GTEST_DIR)/googletest/include -I$(GLFW_DIR)/include -I$(GLAD_DIR)/out/include -I$(COMMON_DIR)
BENCHMARK_INCLUDE_PATHS=-Isrc/ -I$(GBENCHMARK_DIR)/include -I$(GLFW_DIR)/include -I$(GLAD_DIR)/out/include -I$(COMMON_DIR)
GLSL_ALGO_DIR=src/glsl_algo
TEST_DIR=src/tests
BENCHMARK_DIR=src/benchmark
COMMON_DIR=src/common
OUTPUT_DIR=out
MKDIR_C=mkdir -p

.PHONY: clean create_out_dir configure

all: build test benchmark

create_out_dir:
	$(MKDIR_C) out

configure:
	python src/configurator/basic_configurator.py
	
build: configure create_out_dir libglslalgo.a

#library
libglslalgo.a: prefix_scan.o radix_sort.o reduce.o init.o scan.o
	$(AR) $(OUTPUT_DIR)/libglslalgo.a $(OUTPUT_DIR)/prefix_scan.o $(OUTPUT_DIR)/radix_sort.o $(OUTPUT_DIR)/reduce.o $(OUTPUT_DIR)/scan.o $(OUTPUT_DIR)/init.o
	
prefix_scan.o: $(GLSL_ALGO_DIR)/prefix_scan.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/prefix_scan.c -o $(OUTPUT_DIR)/prefix_scan.o
	
radix_sort.o: $(GLSL_ALGO_DIR)/radix_sort.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/radix_sort.c -o $(OUTPUT_DIR)/radix_sort.o

reduce.o: $(GLSL_ALGO_DIR)/shaders/reduce.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/shaders/reduce.c -o $(OUTPUT_DIR)/reduce.o

scan.o: $(GLSL_ALGO_DIR)/shaders/scan.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/shaders/scan.c -o $(OUTPUT_DIR)/scan.o

init.o: $(GLSL_ALGO_DIR)/init.c
	$(CC) $(CFLAGS) -Idependencies $(GLSL_ALGO_DIR)/init.c -o $(OUTPUT_DIR)/init.o

#tests
test: build gtest common test_main.o prefix_scan_tests.o util_tests.o
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

#benchmark
benchmark: gbenchmark common benchmark_main.o prefix_scan_benchmark.o
	$(CCP) -o $(OUTPUT_DIR)/benchmark_main $(OUTPUT_DIR)/benchmark_main.o $(OUTPUT_DIR)/prefix_scan_benchmark.o $(OUTPUT_DIR)/gl_setup.o -L$(OUTPUT_DIR) -lglslalgo -lbenchmark -lglad -lglfw3 -lpthread -lX11 -ldl -lXrandr -lXi -lXinerama -lXcursor -lGL
	
benchmark_main.o: $(BENCHMARK_DIR)/main.cpp
	$(CCP) $(CPPFLAGS) $(BENCHMARK_INCLUDE_PATHS) $(BENCHMARK_DIR)/main.cpp -o $(OUTPUT_DIR)/benchmark_main.o

prefix_scan_benchmark.o: $(BENCHMARK_DIR)/prefix_scan_benchmark.cpp
	$(CCP) $(CPPFLAGS) $(BENCHMARK_INCLUDE_PATHS) $(BENCHMARK_DIR)/prefix_scan_benchmark.cpp -o $(OUTPUT_DIR)/prefix_scan_benchmark.o

gbenchmark:
	cd $(GBENCHMARK_DIR) && mkdir -p out && cd out && cmake ../ && make
	cp $(GBENCHMARK_DIR)/out/src/libbenchmark.a $(OUTPUT_DIR)/libbenchmark.a

#common
common: glfw glad gl_setup.o

gl_setup.o: $(COMMON_DIR)/gl_setup.cpp
	$(CCP) $(CPPFLAGS) -Isrc/ -I$(GLFW_DIR)/include -I$(GLAD_DIR)/out/include $(COMMON_DIR)/gl_setup.cpp -o $(OUTPUT_DIR)/gl_setup.o

glfw:
	cd $(GLFW_DIR)/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GLFW_DIR)/out/src/libglfw3.a $(OUTPUT_DIR)/libglfw3.a

glad:
	cd $(GLAD_DIR)/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GLAD_DIR)/out/libglad.a $(OUTPUT_DIR)/libglad.a

clean:
		rm -rf $(OUTPUT_DIR)/
		rm -rf $(GTEST_DIR)/googletest/out
		rm -rf $(GBENCHMARK_DIR)/benchmark/out
		rm -rf $(GLFW_DIR)/out
		rm -rf $(GLAD_DIR)/out