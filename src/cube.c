#include "cube.h"

Cube *cube_create(vec3 position, vec3 dimensions) {
    Cube *cube = malloc(sizeof(struct cube));
    glm_vec3_copy(position, cube->position);
    glm_vec3_copy(dimensions, cube->dimensions);

    cube->up = (Face) {{1.0f, 1.0f, 1.0f}};
    cube->down = (Face) {{1.0f, 1.0f, 0.0f}};
    cube->left = (Face) {{1.0f, 0.5f, 0.0f}};
    cube->right = (Face) {{1.0f, 0.0f, 0.0f}};
    cube->front = (Face) {{0.0f, 1.0f, 0.0f}};
    cube->back = (Face) {{0.0f, 0.0f, 1.0f}};

    return cube;
}

void cube_free(Cube *cube) {
    free(cube);
}

void cube_set_color(Cube *cube, vec3 color) {
    glm_vec3_copy(color, cube->up.color);
    glm_vec3_copy(color, cube->down.color);
    glm_vec3_copy(color, cube->left.color);
    glm_vec3_copy(color, cube->right.color);
    glm_vec3_copy(color, cube->front.color);
    glm_vec3_copy(color, cube->back.color);
}

void set_face(vec3 color, float *color_data, int *ptr) {
    glm_vec3_copy(color, color_data + *ptr);
    glm_vec3_copy(color, color_data + *ptr + 3);
    glm_vec3_copy(color, color_data + *ptr + 6);
    glm_vec3_copy(color, color_data + *ptr + 9);
    *ptr += 12;
}

void cube_color_data(Cube *cube, float *color_data) {
    int ptr = 0;
    set_face(cube->front.color, color_data, &ptr);
    set_face(cube->back.color, color_data, &ptr);
    set_face(cube->left.color, color_data, &ptr);
    set_face(cube->right.color, color_data, &ptr);
    set_face(cube->up.color, color_data, &ptr);
    set_face(cube->down.color, color_data, &ptr);
}