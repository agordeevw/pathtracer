# pathtracer
Implementing Peter Shirley's raytracing books.

# Building

Project requires installed CMake and Conan package manager.

Run these commands from root directory:
```
mkdir build
cd build
conan install -s build_type=Release ..
cmake ..
cmake --build .
```

# Progress:

## "Raytracing in One Weekend"

Done.

Final result:
![RTIOW-final](https://github.com/agordeevw/pathtracer/blob/master/images/RTIOW-final.png)

## "Raytracing: The Next Week"

In progress.

Motion blur + BVH:
![MotionBlur-BVH](https://github.com/agordeevw/pathtracer/blob/master/images/MotionBlur-BVH.png)

Slightly flawed method of scene generation resulted in two spheres right at the center merging together. Rendering this image with 512 samples per pixel using BVH took ~1000 seconds on my laptop's CPU (i5-4200U). Also glass spheres confuse me :|

## "Raytracing: The Rest Of Your Life"

Pending.
