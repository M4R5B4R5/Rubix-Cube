#ifndef RUBIX_H
#define RUBIX_H

#define RUBIX_SIZE 3
#define RUBIX_GAP 0.05f
#define N_CUBES (RUBIX_SIZE * RUBIX_SIZE * RUBIX_SIZE)
#define CUBE_SIZE 1.5f

#define PLANE_SIZE (RUBIX_SIZE * RUBIX_SIZE)
#define XY_PLANE (ivec3) {1, 1, 0}
#define XZ_PLANE (ivec3) {1, 0, 1}
#define YZ_PLANE (ivec3) {0, 1, 1}

// As seen from a position at a greater positive value
#define CLOCKWISE 1
#define ANTICLOCKWISE -1

#define TWIST_SPEED 0.1f

#include <stdlib.h>
#include "cube.h"
#include "cube_renderer.h"

typedef ivec3 ptype;

typedef union {
    Cube *arr[N_CUBES];
    Cube *map[RUBIX_SIZE][RUBIX_SIZE][RUBIX_SIZE];
} cube_t;

typedef union {
    Cube *arr[PLANE_SIZE];
    Cube *map[RUBIX_SIZE][RUBIX_SIZE];
} plane_t;

typedef struct plane {
    float angle;
    float next_angle;
    float theta;

    plane_t cubes;
    ptype type;
    vec3 local;
} Plane;

typedef struct rubix {
    vec3 position;
    cube_t cubes;

    Plane active_plane;
} Rubix;

// Transform world-based coordinates into a system where the center of the rubix cube
// is the origin of this space
void localize(ivec3 position, vec3 out);
void delocalize(vec3 local, ivec3 out);
Plane rotate_plane(Rubix *rubix);

Rubix *rubix_create(vec3 position);
void rubix_free(Rubix *rubix);

Cube *rubix_get_cube(Rubix *rubix, vec3 local);
Plane rubix_plane(Rubix *rubix, ivec3 plane, int local);
void rubix_render(Rubix *rubix, CubeRenderer *renderer);
void rubix_twist(Rubix *rubix, ptype type, int local, int direction);
void rubix_rotate(Rubix *rubix);

#endif