# Symbios

## What is this?

Gyro is a framework to make 3D visual effects.

## Target
For now the target is windows platform.
https://github.com/alaingalvan/CrossWindow
(can't build a static/dynamic library out of it so fuck it)

## Stepping stones
* [X] ~~Create a win32 basic window~~
* [X] ~~wrap it in c++ classes~~
* [X] ~~strip unnecessary code/features (like the about callback)~~
* [X] Create an opengl context window
* [X] Bind vulkan to it
* [X] Decide on what GPU/3D API to use (I choose you, vulkan! why? because you can optimize and have greater control over everything)
* [ ] Clean vulkan warnings/errors & restructure code

## Resources

### Libraries
* https://www.raylib.com/ - raylib is a simple and easy-to-use library to enjoy videogames programming. 
might be good for inspiration regarding drawing functions etc.
* https://skia.org/ - 2d graphics engine powering firefox/chrome. Alot of inspiration regarding antialiasing and other stuff.

### GPU Contexts/API
* https://www.vulkan.org/
* https://docs.microsoft.com/en-us/windows/win32/direct3d
* https://www.opengl.org/

### Programming C/C++
* https://www.enlyze.com/blog/writing-win32-apps-like-its-2020/part-2/

### Documentation
* https://vulkan.lunarg.com/

### Findings
* Creating a regular old win32 window doesnt cut it. You need to create the window with https://www.glfw.org/. Vulkan on it's own cant do this.
* When a window is created a vulkan instance is then created and bound to the window if i understand correctly.
* https://alain.xyz/blog/comparison-of-modern-graphics-apis

### Similar projects
* https://github.com/mosra/magnum
* https://github.com/amzeratul/halley

### Organization Tips ( Since vulkan requires alot of code )
* https://www.reddit.com/r/vulkan/comments/78ousv/how_to_organize_my_vulkan_code/
* https://github.com/Noxagonal/Vulkan-API-Tutorials
* https://en.cppreference.com/w/cpp/language/raii
* https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization

### Open issues
* [ ] Which 3D file format should we use?
* [ ] Music synchronization. How do we sync our demo with the music?
* [ ] Should we use a test-driven approach?

### Closed issues
* [X] GLM added to the repo - it contains math functions (matrix multiplication etc)
* [X] Added a small trace/logging lib
* [X] Added a library for loading .obj files
* [X] Added a library for loading textures (.png, .jpg etc..)