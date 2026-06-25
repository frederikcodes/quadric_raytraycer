cmake_minimum_required(VERSION 3.16)

project(QuadricRaytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TBB REQUIRED)

set(RAYTRACER_SOURCES
    src/acceleration/AABB.cpp
    src/acceleration/Octree.cpp
    src/acceleration/OctreeNode.cpp
    src/core/Camera.cpp
    src/core/Material.cpp
    src/core/Ray.cpp
    src/core/Vec3.cpp
    src/debug/AABBDebug.cpp
    src/hittable/Hittable.cpp
    src/hittable/HittableList.cpp
    src/primitives/Cone.cpp
    src/primitives/Cylinder.cpp
    src/primitives/Ellipsoid.cpp
    src/primitives/Sphere.cpp
    src/scenes/Scenes.cpp
    src/transforms/RotateX.cpp
    src/transforms/RotateZ.cpp
    src/transforms/Translate.cpp
)

add_executable(raytracer
    src/main.cpp
    ${RAYTRACER_SOURCES}
)

add_executable(raytracer_tbb
    src/main_tbb.cpp
    ${RAYTRACER_SOURCES}
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
