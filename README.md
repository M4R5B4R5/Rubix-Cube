# Rubix-Cube

A simple interactive 3D rubix cube program. 

## Build Instructions

For now, libraries have not been included in the project source (hopefully in future). 
You will need to install:
- GLFW https://www.glfw.org/
- CGLM https://github.com/recp/cglm
- GLAD https://glad.dav1d.de/

### On Linux, run the following command in the root project directory. (It may prompt you to install additional dependencies based on your distribution)

```
./build
```

This will automatically build and run the program.

## Interaction Guide-Lines

- To rotate a part of the cube, select an axis of rotation ("x", "y", or "z" on your keyboard) and then type
the 1-based index of the plane you would like to rotate around the selected axis.
- Use SPACEBAR to toggle between CLOCKWISE and ANTICLOCKWISE rotations (as viewed from the positively selected axis)
