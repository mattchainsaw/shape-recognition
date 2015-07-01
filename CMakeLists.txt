cmake_minimum_required(VERSION 2.8)
project(shape_recognition)

# .h directory list
include_directories(${CMAKE_SOURCE_DIR}/include)
# .cpp directory list
link_directories(${CMAKE_SOURCE_DIR}/src)        # -L flags for linker

set(SOURCE_FILES
    src/medialAxis.cpp
    src/Image.cpp)

add_executable(shape_recognition ${SOURCE_FILES})

target_link_libraries(shape_recognition CGAL gmp)   # -l flags for linking prog target
