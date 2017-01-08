CC=gcc
CCP=g++
AR=ar -rcs
CFLAGS=-c -Wall -Werror
CPPFLAGS=-c -Wall -Werror -std=c++11 -Isrc/glsl_algo/ -Idependencies/googletest/googletest/include
GLSL_ALGO_DIR=src/glsl_algo
TEST_DIR=src/tests
OUTPUT_DIR=out
GTEST_DIR=dependencies/googletest
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
test: build gtest test_main.o prefix_scan_tests.o
	$(CCP) -o $(OUTPUT_DIR)/test_main $(OUTPUT_DIR)/test_main.o $(OUTPUT_DIR)/prefix_scan_tests.o -L$(OUTPUT_DIR) -lglslalgo -lgtest -pthread

test_main.o: $(TEST_DIR)/main.cpp
	$(CCP) $(CPPFLAGS) $(TEST_DIR)/main.cpp -o $(OUTPUT_DIR)/test_main.o

prefix_scan_tests.o: $(TEST_DIR)/prefix_scan_tests.cpp
	$(CCP) $(CPPFLAGS) $(TEST_DIR)/prefix_scan_tests.cpp -o $(OUTPUT_DIR)/prefix_scan_tests.o

gtest:
	cd $(GTEST_DIR)/googletest/ && mkdir -p out && cd out && cmake ../ && make
	cp $(GTEST_DIR)/googletest/out/libgtest.a $(OUTPUT_DIR)/libgtest.a

clean:
		rm -rf $(OUTPUT_DIR)/