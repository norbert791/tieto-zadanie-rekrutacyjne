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
include test/CMakeFiles/Project_run.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/Project_run.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/Project_run.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/Project_run.dir/flags.make

test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o: test/CMakeFiles/Project_run.dir/flags.make
test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o: ../src/circular_buffer.c
test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o: test/CMakeFiles/Project_run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o -MF CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o.d -o CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o -c /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c

test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Project_run.dir/__/src/circular_buffer.c.i"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c > CMakeFiles/Project_run.dir/__/src/circular_buffer.c.i

test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Project_run.dir/__/src/circular_buffer.c.s"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/norbert/Dokumenty/wppt/kukos/projekt/src/circular_buffer.c -o CMakeFiles/Project_run.dir/__/src/circular_buffer.c.s

test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o: test/CMakeFiles/Project_run.dir/flags.make
test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o: ../test/circular_buffer_test.c
test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o: test/CMakeFiles/Project_run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o -MF CMakeFiles/Project_run.dir/circular_buffer_test.c.o.d -o CMakeFiles/Project_run.dir/circular_buffer_test.c.o -c /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c

test/CMakeFiles/Project_run.dir/circular_buffer_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Project_run.dir/circular_buffer_test.c.i"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c > CMakeFiles/Project_run.dir/circular_buffer_test.c.i

test/CMakeFiles/Project_run.dir/circular_buffer_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Project_run.dir/circular_buffer_test.c.s"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/norbert/Dokumenty/wppt/kukos/projekt/test/circular_buffer_test.c -o CMakeFiles/Project_run.dir/circular_buffer_test.c.s

# Object files for target Project_run
Project_run_OBJECTS = \
"CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o" \
"CMakeFiles/Project_run.dir/circular_buffer_test.c.o"

# External object files for target Project_run
Project_run_EXTERNAL_OBJECTS =

test/Project_run: test/CMakeFiles/Project_run.dir/__/src/circular_buffer.c.o
test/Project_run: test/CMakeFiles/Project_run.dir/circular_buffer_test.c.o
test/Project_run: test/CMakeFiles/Project_run.dir/build.make
test/Project_run: test/CMakeFiles/Project_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/norbert/Dokumenty/wppt/kukos/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Project_run"
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/Project_run.dir/build: test/Project_run
.PHONY : test/CMakeFiles/Project_run.dir/build

test/CMakeFiles/Project_run.dir/clean:
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build/test && $(CMAKE_COMMAND) -P CMakeFiles/Project_run.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/Project_run.dir/clean

test/CMakeFiles/Project_run.dir/depend:
	cd /home/norbert/Dokumenty/wppt/kukos/projekt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/norbert/Dokumenty/wppt/kukos/projekt /home/norbert/Dokumenty/wppt/kukos/projekt/test /home/norbert/Dokumenty/wppt/kukos/projekt/build /home/norbert/Dokumenty/wppt/kukos/projekt/build/test /home/norbert/Dokumenty/wppt/kukos/projekt/build/test/CMakeFiles/Project_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/Project_run.dir/depend
