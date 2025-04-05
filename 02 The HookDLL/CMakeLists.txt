# cmake -B Build -A "x64"
# cmake --build Build --config Release

cmake_minimum_required(VERSION 3.10)

project(HookDLL)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(Include)

#Create DLL
add_library(HookDLL SHARED HookDLL.cpp)

# Add the Lib Folder for Libraries to be linked
target_link_directories(HookDLL PUBLIC lib)
target_link_libraries(HookDLL PRIVATE  EasyHook64.lib)

