# Path-tracer-using-OpenGL

A path tracer made using OpenGL with decent UI. This path tracing works with non-polygon graphics, which means that all distance calculations are based on the position and direction of the ray.


![BA0GHUpS](https://github.com/Eduard0110/Path-tracer-using-OpenGL/assets/120855690/a2ac6c3e-0641-4897-b8e5-d5121087f831)
![CAJZ0V4q](https://github.com/Eduard0110/Path-tracer-using-OpenGL/assets/120855690/fbcd8508-6409-437a-94cd-ba0d8986d5da)
![rsu6tCdx](https://github.com/Eduard0110/Path-tracer-using-OpenGL/assets/120855690/f21eb4d4-a7df-4af5-8363-63ea44306c0d)

#### Compilation

##### Unix
need to install ``glfw`` using your package manager 
- ``sh build.sh``
- run with ``./path_tracer``

Features
* 3 different types of object - Cube, Sphere and Plane
* 3 types of materials - Rougth - Metalic, Lens, Light source (emissive material)
* Depth Of Field effect
* Accumulation of previous frames to denoise the image
* You can load any images you want for the skybox (.png or .hdr)


Controls
* W, A, S, D, left Shift and Space - Move the camera
* Mouse - Rotate the camera
* E - Disable or Enable the camera movement
* R - Disable or Enable the Render mode
* F - Hide or Display the User Interface
* T - Make a screenshot of the scene


Useful links that helped me with this project
* Ray-Surface intersection functions https://iquilezles.org/articles/intersectors/
* Cool article about path tracing https://blog.demofox.org/2016/09/21/path-tracing-getting-started-with-diffuse-and-emissive/
* And one more https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
