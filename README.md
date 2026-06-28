# CPU-Based Raytracer for Rendering Quadric Objects

This project implements a CPU-based ray tracing renderer developed for the High Performance Computing module.

The renderer follows a classical recursive ray tracing approach and supports different quadric primitives, transformations, lighting effects, and spatial acceleration. Two executables are provided:

* **raytracer** – single-threaded CPU renderer
* **raytracer_tbb** – multi-threaded CPU renderer using Intel Threading Building Blocks (TBB)

## Features

* CPU-based recursive ray tracing
* Quadric primitives:

  * Sphere
  * Ellipsoid
  * Cylinder
  * Cone
* Phong illumination model

  * Ambient lighting
  * Diffuse (Lambert) reflection
  * Specular highlights
* Shadow rays
* Recursive reflections
* Anti-aliasing using stochastic supersampling
* Object transformations

  * Translation
  * Rotation
* Octree acceleration structure
* Axis-aligned bounding boxes (AABB)
* Optional multi-threaded rendering using Intel TBB

## Architecture

The renderer uses an object-oriented architecture based on a common `Hittable` interface. All scene objects implement the same intersection interface, allowing primitives, transformed objects, and acceleration structures to be processed uniformly.

Spatial acceleration is implemented using an octree to reduce unnecessary ray-object intersection tests.

The project contains two executables that share the same rendering code:

* **raytracer** uses a single CPU thread.
* **raytracer_tbb** parallelizes image rendering using Intel Threading Building Blocks (TBB).

## Build

The project uses CMake.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

Run the single-threaded version:

```bash
./raytracer
```

Run the multi-threaded TBB version:

```bash
./raytracer_tbb
```

The rendered image is written to the output file defined in the application.

## Reference

This project was inspired by:

Peter Shirley
*Ray Tracing in One Weekend*
Version 4.0.2 (2025)
https://raytracing.github.io/books/RayTracingInOneWeekend.html

## Author

Frederik Mauren
