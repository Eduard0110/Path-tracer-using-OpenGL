# Path-tracer-using-OpenGL

A path tracer made using OpenGL with decent UI. This path tracing works with non-polygon graphics, which means that all distance calculations are based on the position and direction of the ray.

![BA0GHUpS](https://github.com/Eduard0110/Path-tracer-using-OpenGL/assets/120855690/8b3f7784-f27d-4543-887c-fae5010aa48d)


For those who wants to run this project I have built the project and added it to this repository in the "Executable folder". Before you run it just move the "images/", "shaders/" and "scenes/" folders to the "Executable/" because those files have to be in the same directory as the the .exe itself for it to run.


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
