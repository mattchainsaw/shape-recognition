cmake_minimum_required(VERSION 2.8)
project(shape_recognition)

# .h directory list
include_directories(${CMAKE_SOURCE_DIR}/include)
# .cpp directory list
link_directories(${CMAKE_SOURCE_DIR}/src)        # -L flags for linker
# flags
add_definitions(-std=c++11)

add_executable(shape_recognition src/main.cpp)

target_link_libraries(shape_recognition CGAL gmp)   # -l flags for linking to exe target
