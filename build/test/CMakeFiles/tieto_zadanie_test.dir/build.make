# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_SOURCE_DIR = /home/norbert/Dokumenty/wppt/kukos/projekt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/norbert/Dokumenty/wppt/kukos/projekt/build

# Include any dependencies generated for this target.
include test/CMakeFiles/tieto_zadanie_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/tieto_zadanie_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/tieto_zadanie_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/tieto_zadanie_test.dir/flags.make

test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o: test/CMakeFiles/tieto_zadanie_test.dir/flags.make
test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o: ../src/circular_buffer.c
test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o: test/CMakeFiles/tieto_zadanie_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o -MF CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o.d -o CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o -c /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c

test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.i"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c > CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.i

test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.s"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c -o CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.s

test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o: test/CMakeFiles/tieto_zadanie_test.dir/flags.make
test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o: ../test/circular_buffer_test.c
test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o: test/CMakeFiles/tieto_zadanie_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o -MF CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o.d -o CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o -c /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c

test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.i"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c > CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.i

test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.s"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c -o CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.s

# Object files for target tieto_zadanie_test
tieto_zadanie_test_OBJECTS = \
"CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o" \
"CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o"

# External object files for target tieto_zadanie_test
tieto_zadanie_test_EXTERNAL_OBJECTS =

test/tieto_zadanie_test: test/CMakeFiles/tieto_zadanie_test.dir/__/src/circular_buffer.c.o
test/tieto_zadanie_test: test/CMakeFiles/tieto_zadanie_test.dir/circular_buffer_test.c.o
test/tieto_zadanie_test: test/CMakeFiles/tieto_zadanie_test.dir/build.make
test/tieto_zadanie_test: test/CMakeFiles/tieto_zadanie_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable tieto_zadanie_test"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tieto_zadanie_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/tieto_zadanie_test.dir/build: test/tieto_zadanie_test
.PHONY : test/CMakeFiles/tieto_zadanie_test.dir/build

test/CMakeFiles/tieto_zadanie_test.dir/clean:
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && $(CMAKE_COMMAND) -P CMakeFiles/tieto_zadanie_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/tieto_zadanie_test.dir/clean

test/CMakeFiles/tieto_zadanie_test.dir/depend:
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/norbert/Dokumenty/wppt/kukos/projekt /home/norbert/Dokumenty/wppt/kukos/projekt/test /home/norbert/Dokumenty/wppt/kukos/projekt/build /home/norbert/Dokumenty/wppt/kukos/projekt/build/test /home/norbert/Dokumenty/wppt/kukos/projekt/build/test/CMakeFiles/tieto_zadanie_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/tieto_zadanie_test.dir/depend
