# Paradime3D
A real-time 3D rendering framework that I worked on alone during my free time outside the university. 
The project was started for learning purposes: to expand my knowledge beyond what the university course could offer; to gain experience in developing a long-term, big scale project; and to serve as a testing ground for new implementations I am working on. 
I focused on creating my own implementations, instead of using 3rd party libraries, to essentially learn the programming areas that are relevant to my field.

![paradime3d-1](https://i.imgur.com/zcAHGg7.png)

## Summary

 - 3D model rendering with texture mapping
 - Billboards (constructed in a geometry shader)
 - Deferred renderer (using data-sets for additional decoupling)
 - Dynamic sky color with day/night transitions
 - Phong lighting with stencil culling (directional, point and spot lights)
 - 3D math (custom written math functionality)
 - Adaptive shader uniforms (complete shader changes without framework recompilation)
 - Data-driven design (all data contained in config/map files)
 - Modular design (different systems are decoupled from each other)
 - Scene graph structure used by all game objects

## Graphics:

**Renderer** - OpenGL graphics API is used. Implemented deferred renderer, with modular approach, splitting different effects into separate passes, for easy integration of new effects. Visitor pattern is being used for individual object rendering. Supports geometry, vertex and fragment shaders. Internally uses state machine-like approach (similar to OpenGL) to hold values required for rendering.

**Deferred shading** - uses a g-buffer to decouple the lighting calculations from geometry rendering. Shading is carried out on visible pixels only, thus eliminating unnecessary calculations and handling any depth complexity. This allows for a high number of dynamic lights in the scene and easy integration of post-processing effects.

**ADS Lighting** - integrated simple phong shading (using lambertian reflectance for diffuse and phong reflectance for specular components). Supports directional, point and spot lights.

**Stencil light culling** - rendering a scaled light geometry (sphere for a point light, cone for a spot light) into a stencil buffer, to carry light calculations on affected pixels only.

**Adaptive shader uniforms** - compiled shaders are checked for any required variables to be sent from the framework. This way, shaders can be drastically changed without the need to recompile the framework.

**Billboards** - sprite rendering by building a scaled billboard in a geometry shader.

**Sky** - dynamic sky with sun and moon positions based on the time of day. Static skybox can also be used instead.

## Engine:

**Framework** - designed to be modular and extendable. Everything is data driven, meaning no hard-coded values are used, instead they are loaded from configuration files (which allows modifying values without the need to recompile the code).

**Game states** - easy switching between different engine states, while saving the current conditions (for example switching between menu and gameplay states).

**3D Math library** - all the required 3D math functionality is written specifically for the framework and graphics API, without using any external libraries.

**Asset loaders** - supporting multiple formats of textures and models.

**JSON loader** - general-purpose JSON file loader, used for maps and configuration files.

**Data sets** - when rendering is called upon objects, their data is copied and put into data sets to be rendered later. In effect, it decouples rendering from everything else, useful for multithreading.

**OS Layer decoupling** - operating system specific functionality (window, mouse and keyboard events, etc) are decoupled for easy integration into different APIs / OSes.

**Error handling** - messages from around the framework are sent to the error handler which manages them accordingly to their source and the severity of an error (for instance outputting to the console for a warning, shutting down the framework and popping up a message window for a fatal error)..

**Scene graph** - objects are contained in a scene graph structure for easy special coupling and transformation.

**Window** - toggling of vertical synchronization, mouse capturing and full screen view. Window can be resized (to any resolution), which sets the projection and viewport values accordingly, to keep everything in perspective.


## Screenshots:


![paradime3d-2](https://i.imgur.com/QPJLAsW.png)
![paradime3d-3](https://i.imgur.com/uNBRZgb.png)
![paradime3d-4](https://i.imgur.com/d8Ml3dz.png)


## Dependencies
Libraries used in this project:  

- [freeglut](https://freeglut.sourceforge.net/)
- [GLEW](https://glew.sourceforge.net/)
- [GLFW](https://www.glfw.org/)
- [Open-Asset-Importer-Lib (ASSIMP)](https://github.com/assimp/assimp)
- [FreeImage](https://freeimage.sourceforge.io/)
- [SDL](https://www.libsdl.org/)
    
Their individual licenses can be found in **LICENSE.md** file.


## License

**The software is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License:**

Copyright © 2020 Paulius Akulavicius

Permission is hereby granted, free of charge, 
to any person obtaining a copy of this software 
and associated documentation files (the “Software”), 
to deal in the Software without restriction, 
including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions 
of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.