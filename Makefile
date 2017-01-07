CC=gcc
AR=ar -rcs
CFLAGS=-c -Wall -Werror
CPPFLAGS=$(CFLAGS) -xc++ -lstdc++ -shared-libgcc -std=c++11 -Isrc/glsl_algo/
GLSL_ALGO_DIR=src/glsl_algo
TEST_DIR=src/tests
OUTPUT_DIR=out
MKDIR_C=mkdir -p

.PHONY: clean create_out_dir configure

all: build benchmark test

create_out_dir:
	$(MKDIR_C) out

configure:
	python src/configurator/basic_configurator.py
	
build: configure create_out_dir libglslalgo.a

test: build test_main.o
	$(CC) -o $(OUTPUT_DIR)/test_main $(OUTPUT_DIR)/test_main.o -L$(OUTPUT_DIR) -lglslalgo
	
prefix_scan.o: $(GLSL_ALGO_DIR)/prefix_scan.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/prefix_scan.c -o $(OUTPUT_DIR)/prefix_scan.o
	
radix_sort.o: $(GLSL_ALGO_DIR)/radix_sort.c
	$(CC) $(CFLAGS) $(GLSL_ALGO_DIR)/radix_sort.c -o $(OUTPUT_DIR)/radix_sort.o

libglslalgo.a: prefix_scan.o radix_sort.o
	$(AR) $(OUTPUT_DIR)/libglslalgo.a $(OUTPUT_DIR)/prefix_scan.o $(OUTPUT_DIR)/radix_sort.o

test_main.o: $(TEST_DIR)/main.cpp
	$(CC) $(CPPFLAGS) $(TEST_DIR)/main.cpp -o $(OUTPUT_DIR)/test_main.o

clean:
		rm -rf $(OUTPUT_DIR)/