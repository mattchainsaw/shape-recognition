cmake_minimum_required(VERSION 2.8)
project(shape_recognition)

# .h directory list
include_directories(${CMAKE_SOURCE_DIR}/include)
# .cpp directory list
link_directories(${CMAKE_SOURCE_DIR}/src)        # -L flags for linker
# flags
add_definitions(-std=c++11)

set(FILES src/Medial.cpp src/main.cpp)

add_executable(shape_recognition ${FILES})

target_link_libraries(shape_recognition CGAL gmp)   # -l flags for linking to exe target
