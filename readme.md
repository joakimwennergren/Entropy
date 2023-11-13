# Symbios

## What is this?

Entropy..

## Target

## Resources

### Libraries
* https://www.raylib.com/ - raylib is a simple and easy-to-use library to enjoy videogames programming. 
might be good for inspiration regarding drawing functions etc.
* https://skia.org/ - 2d graphics engine powering firefox/chrome. Alot of inspiration regarding antialiasing and other stuff.

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