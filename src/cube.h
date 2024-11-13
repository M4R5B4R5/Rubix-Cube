#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct face {
    vec3 color;
} Face;

typedef struct cube {
    vec3 position;
    vec3 dimensions;
    vec3 axis;
    
    float angle;
    float nextAngle;
    
    Face up;
    Face down;
    Face left;
    Face right;
    Face front;
    Face back;
} Cube;

Cube *cube_create(vec3 position, vec3 dimensions);
void cube_free(Cube *cube);

void cube_color_data(Cube *cube, float *color_data);
void cube_set_color(Cube *cube, vec3 color);

#endif