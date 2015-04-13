cmake_minimum_required (VERSION 2.6)
project (LogScanner C CXX)

SET_DIRECTORY_PROPERTIES(PROPERTIES 
   ADDITIONAL_MAKE_CLEAN_FILES "cmake_install.cmake;CMakeCache.txt;Makefile") 

set (LogScanner_VERSION_MAJOR 0)
set (LogScanner_VERSION_MINOR 0)
set (LogScanner_VERSION_PATCH 0)

set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set (BUILD_SHARED_LIBRARIES OFF)
set (CMAKE_EXE_LINKER_FLAGS "-static")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")

find_package (Threads REQUIRED)

add_library (filescanner STATIC src/FileScanner.cpp src/FileScanner.h)
add_executable (logscanner src/logscanner.cpp)
target_link_libraries (logscanner filescanner)
