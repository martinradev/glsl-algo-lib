# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/martin/Desktop/code/radix-sort-lib/common

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/martin/Desktop/code/radix-sort-lib/common/build

# Include any dependencies generated for this target.
include glfw/tests/CMakeFiles/sharing.dir/depend.make

# Include the progress variables for this target.
include glfw/tests/CMakeFiles/sharing.dir/progress.make

# Include the compile flags for this target's objects.
include glfw/tests/CMakeFiles/sharing.dir/flags.make

glfw/tests/CMakeFiles/sharing.dir/sharing.c.o: glfw/tests/CMakeFiles/sharing.dir/flags.make
glfw/tests/CMakeFiles/sharing.dir/sharing.c.o: /home/martin/Desktop/code/radix-sort-lib/deps/glfw/tests/sharing.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/Desktop/code/radix-sort-lib/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw/tests/CMakeFiles/sharing.dir/sharing.c.o"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sharing.dir/sharing.c.o   -c /home/martin/Desktop/code/radix-sort-lib/deps/glfw/tests/sharing.c

glfw/tests/CMakeFiles/sharing.dir/sharing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sharing.dir/sharing.c.i"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/martin/Desktop/code/radix-sort-lib/deps/glfw/tests/sharing.c > CMakeFiles/sharing.dir/sharing.c.i

glfw/tests/CMakeFiles/sharing.dir/sharing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sharing.dir/sharing.c.s"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/martin/Desktop/code/radix-sort-lib/deps/glfw/tests/sharing.c -o CMakeFiles/sharing.dir/sharing.c.s

glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.requires

glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.provides: glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/sharing.dir/build.make glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.provides

glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.provides.build: glfw/tests/CMakeFiles/sharing.dir/sharing.c.o


glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o: glfw/tests/CMakeFiles/sharing.dir/flags.make
glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o: /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/Desktop/code/radix-sort-lib/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sharing.dir/__/deps/getopt.c.o   -c /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/getopt.c

glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sharing.dir/__/deps/getopt.c.i"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/getopt.c > CMakeFiles/sharing.dir/__/deps/getopt.c.i

glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sharing.dir/__/deps/getopt.c.s"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/getopt.c -o CMakeFiles/sharing.dir/__/deps/getopt.c.s

glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.requires

glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.provides: glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/sharing.dir/build.make glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.provides

glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.provides.build: glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o


glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o: glfw/tests/CMakeFiles/sharing.dir/flags.make
glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o: /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/Desktop/code/radix-sort-lib/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sharing.dir/__/deps/glad.c.o   -c /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/glad.c

glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sharing.dir/__/deps/glad.c.i"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/glad.c > CMakeFiles/sharing.dir/__/deps/glad.c.i

glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sharing.dir/__/deps/glad.c.s"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/martin/Desktop/code/radix-sort-lib/deps/glfw/deps/glad.c -o CMakeFiles/sharing.dir/__/deps/glad.c.s

glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.requires

glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.provides: glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/sharing.dir/build.make glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.provides

glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.provides.build: glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o


# Object files for target sharing
sharing_OBJECTS = \
"CMakeFiles/sharing.dir/sharing.c.o" \
"CMakeFiles/sharing.dir/__/deps/getopt.c.o" \
"CMakeFiles/sharing.dir/__/deps/glad.c.o"

# External object files for target sharing
sharing_EXTERNAL_OBJECTS =

glfw/tests/sharing: glfw/tests/CMakeFiles/sharing.dir/sharing.c.o
glfw/tests/sharing: glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o
glfw/tests/sharing: glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o
glfw/tests/sharing: glfw/tests/CMakeFiles/sharing.dir/build.make
glfw/tests/sharing: glfw/src/libglfw3.a
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/librt.so
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/libm.so
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/libX11.so
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/libXrandr.so
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/libXinerama.so
glfw/tests/sharing: /usr/lib/x86_64-linux-gnu/libXcursor.so
glfw/tests/sharing: glfw/tests/CMakeFiles/sharing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/martin/Desktop/code/radix-sort-lib/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable sharing"
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sharing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw/tests/CMakeFiles/sharing.dir/build: glfw/tests/sharing

.PHONY : glfw/tests/CMakeFiles/sharing.dir/build

glfw/tests/CMakeFiles/sharing.dir/requires: glfw/tests/CMakeFiles/sharing.dir/sharing.c.o.requires
glfw/tests/CMakeFiles/sharing.dir/requires: glfw/tests/CMakeFiles/sharing.dir/__/deps/getopt.c.o.requires
glfw/tests/CMakeFiles/sharing.dir/requires: glfw/tests/CMakeFiles/sharing.dir/__/deps/glad.c.o.requires

.PHONY : glfw/tests/CMakeFiles/sharing.dir/requires

glfw/tests/CMakeFiles/sharing.dir/clean:
	cd /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/sharing.dir/cmake_clean.cmake
.PHONY : glfw/tests/CMakeFiles/sharing.dir/clean

glfw/tests/CMakeFiles/sharing.dir/depend:
	cd /home/martin/Desktop/code/radix-sort-lib/common/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/martin/Desktop/code/radix-sort-lib/common /home/martin/Desktop/code/radix-sort-lib/deps/glfw/tests /home/martin/Desktop/code/radix-sort-lib/common/build /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests /home/martin/Desktop/code/radix-sort-lib/common/build/glfw/tests/CMakeFiles/sharing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw/tests/CMakeFiles/sharing.dir/depend
