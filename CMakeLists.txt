cmake_minimum_required(VERSION 3.16)

project(QuadricRaytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TBB REQUIRED)

add_executable(raytracer
    src/main.cpp
    src/scenes/Scenes.cpp
    src/acceleration/Octree.cpp
)

add_executable(raytracer_tbb
    src/main_tbb.cpp
    src/scenes/Scenes.cpp
    src/acceleration/Octree.cpp
)

target_include_directories(raytracer PRIVATE
    include
    src
)

target_include_directories(raytracer_tbb PRIVATE
    include
    src
)

target_link_libraries(raytracer_tbb PRIVATE TBB::tbb)