# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/atty/Prog/libOrientation/trunk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/atty/Prog/libOrientation/trunk/qtcreator-build

# Include any dependencies generated for this target.
include CMakeFiles/TestOrientationMATIS_Projection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TestOrientationMATIS_Projection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TestOrientationMATIS_Projection.dir/flags.make

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o: CMakeFiles/TestOrientationMATIS_Projection.dir/flags.make
CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o: ../testing/TestOrientationMATIS_Projection.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/atty/Prog/libOrientation/trunk/qtcreator-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@echo "Building CXX object CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o -c /home/atty/Prog/libOrientation/trunk/testing/TestOrientationMATIS_Projection.cpp

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.i: cmake_force
	@echo "Preprocessing CXX source to CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/atty/Prog/libOrientation/trunk/testing/TestOrientationMATIS_Projection.cpp > CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.i

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.s: cmake_force
	@echo "Compiling CXX source to assembly CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/atty/Prog/libOrientation/trunk/testing/TestOrientationMATIS_Projection.cpp -o CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.s

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.requires:
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.requires

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.provides: CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.requires
	$(MAKE) -f CMakeFiles/TestOrientationMATIS_Projection.dir/build.make CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.provides.build
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.provides

CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.provides.build: CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.provides.build

# Object files for target TestOrientationMATIS_Projection
TestOrientationMATIS_Projection_OBJECTS = \
"CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o"

# External object files for target TestOrientationMATIS_Projection
TestOrientationMATIS_Projection_EXTERNAL_OBJECTS =

bin/TestOrientationMATIS_Projection: CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_system-mt.so
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_filesystem-mt.so
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_date_time-mt.so
bin/TestOrientationMATIS_Projection: /usr/local/lib/libproj4.so
bin/TestOrientationMATIS_Projection: /usr/local/lib/libTinyXML_STL.so
bin/TestOrientationMATIS_Projection: lib/libOrientation.a
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_system-mt.so
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_filesystem-mt.so
bin/TestOrientationMATIS_Projection: /usr/lib/libboost_date_time-mt.so
bin/TestOrientationMATIS_Projection: /usr/local/lib/libproj4.so
bin/TestOrientationMATIS_Projection: /usr/local/lib/libTinyXML_STL.so
bin/TestOrientationMATIS_Projection: CMakeFiles/TestOrientationMATIS_Projection.dir/build.make
bin/TestOrientationMATIS_Projection: CMakeFiles/TestOrientationMATIS_Projection.dir/link.txt
	@echo "Linking CXX executable bin/TestOrientationMATIS_Projection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestOrientationMATIS_Projection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TestOrientationMATIS_Projection.dir/build: bin/TestOrientationMATIS_Projection
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/build

CMakeFiles/TestOrientationMATIS_Projection.dir/requires: CMakeFiles/TestOrientationMATIS_Projection.dir/testing/TestOrientationMATIS_Projection.cpp.o.requires
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/requires

CMakeFiles/TestOrientationMATIS_Projection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TestOrientationMATIS_Projection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/clean

CMakeFiles/TestOrientationMATIS_Projection.dir/depend:
	cd /home/atty/Prog/libOrientation/trunk/qtcreator-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/atty/Prog/libOrientation/trunk /home/atty/Prog/libOrientation/trunk /home/atty/Prog/libOrientation/trunk/qtcreator-build /home/atty/Prog/libOrientation/trunk/qtcreator-build /home/atty/Prog/libOrientation/trunk/qtcreator-build/CMakeFiles/TestOrientationMATIS_Projection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TestOrientationMATIS_Projection.dir/depend

