# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/flags.make

CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/flags.make
CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/includes_C.rsp
CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\main.c.obj   -c "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\main.c"

CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\main.c" > CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\main.c.i

CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\main.c" -o CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\main.c.s

# Object files for target Chat_Application_Phase2_Server_V2__c
Chat_Application_Phase2_Server_V2__c_OBJECTS = \
"CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj"

# External object files for target Chat_Application_Phase2_Server_V2__c
Chat_Application_Phase2_Server_V2__c_EXTERNAL_OBJECTS =

Chat_Application_Phase2_Server_V2__c.exe: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/main.c.obj
Chat_Application_Phase2_Server_V2__c.exe: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/build.make
Chat_Application_Phase2_Server_V2__c.exe: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/linklibs.rsp
Chat_Application_Phase2_Server_V2__c.exe: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/objects1.rsp
Chat_Application_Phase2_Server_V2__c.exe: CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Chat_Application_Phase2_Server_V2__c.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/build: Chat_Application_Phase2_Server_V2__c.exe

.PHONY : CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/build

CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/clean

CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c" "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c" "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug" "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug" "C:\Users\alin2\Desktop\Chat_Application_Phase2_Server V2 .c\cmake-build-debug\CMakeFiles\Chat_Application_Phase2_Server_V2__c.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Chat_Application_Phase2_Server_V2__c.dir/depend

