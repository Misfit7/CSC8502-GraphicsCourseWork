# GraphicsCourseWork
# Planet-CSC8502

Using C++ and OpenGL that renders a space scene, that transitions between being on the planet surface, to looking down to the planet from space.

# Demo Link:

Most of the functionality is demonstrated in the following demo Youtube: [video](https://youtu.be/MonSYgP1leY) Bili: [video](https://www.bilibili.com/video/BV1FK4y1r73i/)

# List of Features
## Core:

- Generate terrain using homemade grayscale maps.
- Use textures and bump in the fragment shader for terrain.
- Implement efficient rendering of both opaque and transparent objects using an overall scene graph and scene management.
- Implement frustum culling for primitives.
- Illuminate the scene with a movable directional light source, including computing specular reflection components.
- Implement an autoplay moving camera in the scene.
- Utilize simple meshes and textures, merging them into scene elements such as buildings or planetary rings.
- Render objects with multiple textures using depth values as references.

## Advanced:

- Implement skeletal animation.
- Draw skyboxes, terrains, and character objects in different frame buffers during advanced processing.
- Achieve multi-lighting using Deferred Rendering.
- Realize day-night transitions with advanced lighting using Real-Time Lighting.
- Apply post-processing, such as blur, in specific parts of the camera trajectory (underwater).
- Create environmental effects like rain using particle emitters.
- Achieve water flow animation by changing the MVP matrix over time in the vertex shader.
- Represent shadow maps with cube buildings.
- Implement split-screen effects for rendering two viewpoints simultaneously with variable perspectives.

# Hotkey

## General:

"F1" - Switch to ground scene.  
"F2" - Switch to surface scene.  
"WASD" - Move camera.  
"SHIFT" - Camera up.  
"SPACE" - Camera down.  

## Surface Scene:

“T” – Enable day-night cycle.  
“O” – Enable deferred rendering.  
“P” –  Enable and stop autoplay.  

## Space Scene:

"V" - Switch to different perspectives.  
“F” – Switches the screen to the split screen implementation.  
“P” –  Enable and stop autoplay.  

# Real-Time Lighting

You can see the Lighting scenes below:

![Real-Time Lighting]( https://github.com/Misfit7/GraphicsCourseWork/blob/master/FinalPicture/1.png)
  

# Deferred Rendering with Multiple PointLights

You can see the multi-light scene below:

![Deferred Rendering](https://github.com/Misfit7/GraphicsCourseWork/blob/master/FinalPicture/2.png)

  

# Blur

You can see a preview of the scene with the blur filter added:

![Blur]( https://github.com/Misfit7/GraphicsCourseWork/blob/master/FinalPicture/3.png)

  
# Split Screen

You can see the split scenes below:

![Split Screen](https://github.com/Misfit7/GraphicsCourseWork/blob/master/FinalPicture/4.png)

# Skeletal Animation

You can see the BOSS scenes below:

![Skeletal Animation](https://github.com/Misfit7/GraphicsCourseWork/blob/master/FinalPicture/5.png)

