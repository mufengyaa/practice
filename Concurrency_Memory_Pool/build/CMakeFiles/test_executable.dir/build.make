# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mufeng/c++/Concurrency_Memory_Pool/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mufeng/c++/Concurrency_Memory_Pool/build

# Include any dependencies generated for this target.
include CMakeFiles/test_executable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_executable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_executable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_executable.dir/flags.make

CMakeFiles/test_executable.dir/test1.cpp.o: CMakeFiles/test_executable.dir/flags.make
CMakeFiles/test_executable.dir/test1.cpp.o: /home/mufeng/c++/Concurrency_Memory_Pool/code/test1.cpp
CMakeFiles/test_executable.dir/test1.cpp.o: CMakeFiles/test_executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mufeng/c++/Concurrency_Memory_Pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_executable.dir/test1.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_executable.dir/test1.cpp.o -MF CMakeFiles/test_executable.dir/test1.cpp.o.d -o CMakeFiles/test_executable.dir/test1.cpp.o -c /home/mufeng/c++/Concurrency_Memory_Pool/code/test1.cpp

CMakeFiles/test_executable.dir/test1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_executable.dir/test1.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mufeng/c++/Concurrency_Memory_Pool/code/test1.cpp > CMakeFiles/test_executable.dir/test1.cpp.i

CMakeFiles/test_executable.dir/test1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_executable.dir/test1.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mufeng/c++/Concurrency_Memory_Pool/code/test1.cpp -o CMakeFiles/test_executable.dir/test1.cpp.s

# Object files for target test_executable
test_executable_OBJECTS = \
"CMakeFiles/test_executable.dir/test1.cpp.o"

# External object files for target test_executable
test_executable_EXTERNAL_OBJECTS =

test_executable: CMakeFiles/test_executable.dir/test1.cpp.o
test_executable: CMakeFiles/test_executable.dir/build.make
test_executable: CMakeFiles/test_executable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mufeng/c++/Concurrency_Memory_Pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_executable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_executable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_executable.dir/build: test_executable
.PHONY : CMakeFiles/test_executable.dir/build

CMakeFiles/test_executable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_executable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_executable.dir/clean

CMakeFiles/test_executable.dir/depend:
	cd /home/mufeng/c++/Concurrency_Memory_Pool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mufeng/c++/Concurrency_Memory_Pool/code /home/mufeng/c++/Concurrency_Memory_Pool/code /home/mufeng/c++/Concurrency_Memory_Pool/build /home/mufeng/c++/Concurrency_Memory_Pool/build /home/mufeng/c++/Concurrency_Memory_Pool/build/CMakeFiles/test_executable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_executable.dir/depend

