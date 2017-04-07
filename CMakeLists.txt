cmake_minimum_required(VERSION 2.8.4)
project(llox)

set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -fno-rtti")

include_directories(${CMAKE_SOURCE_DIR}/include)

# Add our own libs.
add_subdirectory(lib)

# Add our own tools.
add_subdirectory(tools)
