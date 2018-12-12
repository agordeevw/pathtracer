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

### Motion blur + BVH:

![MotionBlur-BVH](https://github.com/agordeevw/pathtracer/blob/master/images/MotionBlur-BVH.png)

Slightly flawed method of scene generation resulted in two spheres right at the center merging together. Rendering this image with 512 samples per pixel using BVH took ~1000 seconds on my laptop's CPU (i5-4200U). Also glass spheres confuse me :|

### Debug scene + scene loading from JSON:

![DebugScene+SceneLoading](https://github.com/agordeevw/pathtracer/blob/master/images/DebugScene+SceneLoading.png)

Implemented simple scene loader from JSON description files. Now I don't have to recompile when I want to slightly modify a scene. Less compilation - more happy feelings :)

### Procedural textures (start) + 3D checker pattern

![3D-checker](https://github.com/agordeevw/pathtracer/blob/master/images/Textures-3DChecker.png)

3D-checker pattern works by combining two procedural textures. Right now only constant and checker textures are implemented. Scene description loader should check that texture references digraph is acyclic.

### Noise + procedural texture using the noise

![Noise](https://github.com/agordeevw/pathtracer/blob/master/images/Noise.png)

Implemented procedural Perlin noise and weird procedural texture.

### Image textures

![Image texture](https://github.com/agordeevw/pathtracer/blob/master/images/ImageTexture.png)

Implemented image textures.

## "Raytracing: The Rest Of Your Life"

Pending.
