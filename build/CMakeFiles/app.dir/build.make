# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/brianmasse/Developer/Software/Engine-Remastered

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/brianmasse/Developer/Software/Engine-Remastered/build

# Include any dependencies generated for this target.
include CMakeFiles/app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/app.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app.dir/flags.make

CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/module.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/module.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/module.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/module.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/main.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/main.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/main.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/main.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/validation.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/validation.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/validation.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/validation.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/depthBuffer.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/depthBuffer.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/depthBuffer.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/depthBuffer.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/commands.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/commands.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/commands.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/commands.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/devices.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/devices.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/devices.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/devices.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/draw.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/draw.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/draw.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/draw.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/pipeline.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/pipeline.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/pipeline.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/pipeline.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/swapchain.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/swapchain.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/swapchain.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/app/swapchain.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/object.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/object.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/object.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/object.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexBuffer.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexBuffer.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexBuffer.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexBuffer.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexReader.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexReader.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexReader.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/objects/vertexReader.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/imageReader.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/imageReader.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/imageReader.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/imageReader.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/proxy.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/proxy.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/proxy.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/proxy.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/buffers.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/buffers.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/buffers.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/buffers.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.s

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o: /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/uCommands.cpp
CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o -MF CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o.d -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o -c /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/uCommands.cpp

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/uCommands.cpp > CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.i

CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brianmasse/Developer/Software/Engine-Remastered/Engine-Remastered/Main/universalConstructors/uCommands.cpp -o CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.s

# Object files for target app
app_OBJECTS = \
"CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o" \
"CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o"

# External object files for target app
app_EXTERNAL_OBJECTS =

app: CMakeFiles/app.dir/Engine-Remastered/Main/module.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/main.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/validation.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/depthBuffer.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/commands.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/devices.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/draw.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/pipeline.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/app/swapchain.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/objects/object.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexBuffer.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/objects/vertexReader.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/imageReader.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/proxy.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/buffers.cpp.o
app: CMakeFiles/app.dir/Engine-Remastered/Main/universalConstructors/uCommands.cpp.o
app: CMakeFiles/app.dir/build.make
app: /usr/local/lib/libboost_python311-mt.dylib
app: /Users/brianmasse/Developer/Software/Engine-Remastered/dependencies/library/libglfw.3.3.dylib
app: /Users/brianmasse/Developer/Software/Engine-Remastered/dependencies/library/Vulkan/libvulkan.1.3.231.dylib
app: /Users/brianmasse/opt/miniconda3/lib/libpython3.9.dylib
app: CMakeFiles/app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Linking CXX executable app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app.dir/build: app
.PHONY : CMakeFiles/app.dir/build

CMakeFiles/app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app.dir/clean

CMakeFiles/app.dir/depend:
	cd /Users/brianmasse/Developer/Software/Engine-Remastered/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/brianmasse/Developer/Software/Engine-Remastered /Users/brianmasse/Developer/Software/Engine-Remastered /Users/brianmasse/Developer/Software/Engine-Remastered/build /Users/brianmasse/Developer/Software/Engine-Remastered/build /Users/brianmasse/Developer/Software/Engine-Remastered/build/CMakeFiles/app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/app.dir/depend

