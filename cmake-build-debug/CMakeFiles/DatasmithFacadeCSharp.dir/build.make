# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /opt/clion-2020.3.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.3.1/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/janeyjin/Documents/Unreal Projects/UnrealSumo"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/janeyjin/Documents/Unreal Projects/UnrealSumo/cmake-build-debug"

# Utility rule file for DatasmithFacadeCSharp.

# Include the progress variables for this target.
include CMakeFiles/DatasmithFacadeCSharp.dir/progress.make

CMakeFiles/DatasmithFacadeCSharp:
	cd /home/janeyjin/UnrealEngine-4.24 && bash /home/janeyjin/UnrealEngine-4.24/Engine/Build/BatchFiles/Linux/Build.sh DatasmithFacadeCSharp Linux Development "-project=/home/janeyjin/Documents/Unreal Projects/UnrealSumo/UnrealSumoPlugin.uproject" -game -progress -buildscw

DatasmithFacadeCSharp: CMakeFiles/DatasmithFacadeCSharp
DatasmithFacadeCSharp: CMakeFiles/DatasmithFacadeCSharp.dir/build.make

.PHONY : DatasmithFacadeCSharp

# Rule to build all files generated by this target.
CMakeFiles/DatasmithFacadeCSharp.dir/build: DatasmithFacadeCSharp

.PHONY : CMakeFiles/DatasmithFacadeCSharp.dir/build

CMakeFiles/DatasmithFacadeCSharp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DatasmithFacadeCSharp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DatasmithFacadeCSharp.dir/clean

CMakeFiles/DatasmithFacadeCSharp.dir/depend:
	cd "/home/janeyjin/Documents/Unreal Projects/UnrealSumo/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/janeyjin/Documents/Unreal Projects/UnrealSumo" "/home/janeyjin/Documents/Unreal Projects/UnrealSumo" "/home/janeyjin/Documents/Unreal Projects/UnrealSumo/cmake-build-debug" "/home/janeyjin/Documents/Unreal Projects/UnrealSumo/cmake-build-debug" "/home/janeyjin/Documents/Unreal Projects/UnrealSumo/cmake-build-debug/CMakeFiles/DatasmithFacadeCSharp.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/DatasmithFacadeCSharp.dir/depend

