# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/yduman/GdI3_Prak4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yduman/GdI3_Prak4/build

# Include any dependencies generated for this target.
include CMakeFiles/disasm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/disasm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/disasm.dir/flags.make

CMakeFiles/disasm.dir/disasm/main.cc.o: CMakeFiles/disasm.dir/flags.make
CMakeFiles/disasm.dir/disasm/main.cc.o: ../disasm/main.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yduman/GdI3_Prak4/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/disasm.dir/disasm/main.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/disasm.dir/disasm/main.cc.o -c /home/yduman/GdI3_Prak4/disasm/main.cc

CMakeFiles/disasm.dir/disasm/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disasm.dir/disasm/main.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yduman/GdI3_Prak4/disasm/main.cc > CMakeFiles/disasm.dir/disasm/main.cc.i

CMakeFiles/disasm.dir/disasm/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disasm.dir/disasm/main.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yduman/GdI3_Prak4/disasm/main.cc -o CMakeFiles/disasm.dir/disasm/main.cc.s

CMakeFiles/disasm.dir/disasm/main.cc.o.requires:
.PHONY : CMakeFiles/disasm.dir/disasm/main.cc.o.requires

CMakeFiles/disasm.dir/disasm/main.cc.o.provides: CMakeFiles/disasm.dir/disasm/main.cc.o.requires
	$(MAKE) -f CMakeFiles/disasm.dir/build.make CMakeFiles/disasm.dir/disasm/main.cc.o.provides.build
.PHONY : CMakeFiles/disasm.dir/disasm/main.cc.o.provides

CMakeFiles/disasm.dir/disasm/main.cc.o.provides.build: CMakeFiles/disasm.dir/disasm/main.cc.o

# Object files for target disasm
disasm_OBJECTS = \
"CMakeFiles/disasm.dir/disasm/main.cc.o"

# External object files for target disasm
disasm_EXTERNAL_OBJECTS =

disasm: CMakeFiles/disasm.dir/disasm/main.cc.o
disasm: CMakeFiles/disasm.dir/build.make
disasm: CMakeFiles/disasm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable disasm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/disasm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/disasm.dir/build: disasm
.PHONY : CMakeFiles/disasm.dir/build

CMakeFiles/disasm.dir/requires: CMakeFiles/disasm.dir/disasm/main.cc.o.requires
.PHONY : CMakeFiles/disasm.dir/requires

CMakeFiles/disasm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/disasm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/disasm.dir/clean

CMakeFiles/disasm.dir/depend:
	cd /home/yduman/GdI3_Prak4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yduman/GdI3_Prak4 /home/yduman/GdI3_Prak4 /home/yduman/GdI3_Prak4/build /home/yduman/GdI3_Prak4/build /home/yduman/GdI3_Prak4/build/CMakeFiles/disasm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/disasm.dir/depend

