
# CPU-Based Raytracer for Rendering Quadric Objects

This project implements a CPU-based ray tracing renderer developed for the first milestone of the High Performance Computing module.

The renderer follows a classical recursive ray tracing approach and supports different quadric primitives, transformations, lighting effects, and spatial acceleration.

## Features

- CPU-based recursive ray tracing
- Quadric primitives:
  - Sphere
  - Ellipsoid
  - Cylinder
  - Cone
- Phong illumination model
  - Ambient lighting
  - Diffuse (Lambert) reflection
  - Specular highlights
- Shadow rays
- Recursive reflections
- Anti-aliasing using stochastic supersampling
- Object transformations:
  - Translation
  - Rotation
- Octree acceleration structure
- Axis-aligned bounding boxes (AABB)

## Architecture

The renderer uses an object-oriented architecture based on a common `Hittable` interface.
All scene objects implement the same intersection interface, allowing primitives,
transformed objects, and acceleration structures to be processed uniformly.

Spatial acceleration is implemented using an octree to reduce unnecessary ray-object
intersection tests.

## Build

The project uses CMake.

```bash
mkdir build
cd build
cmake ..
cmake --build .
````

## Run

After compilation:

```bash
./raytracer
```

The rendered image is written to the output file defined in the application.

## Future Work (Milestone 2)

Planned extensions:

* Parallel rendering using Intel TBB
* GPU acceleration using CUDA
* Further runtime optimizations
* Additional rendering features

## Reference

This project was inspired by:

Peter Shirley
*Ray Tracing in One Weekend*
Version 4.0.2 (2025)
[https://raytracing.github.io/books/RayTracingInOneWeekend.html](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

## Author

Frederik Mauren


