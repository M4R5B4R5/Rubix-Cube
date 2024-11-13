#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <glad/glad.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "cube.h"

typedef struct cube_renderer {
    unsigned int va;
    unsigned int position_vb;
    unsigned int color_vb;
    unsigned int eb;

    Shader *shader;
} CubeRenderer;

CubeRenderer *cube_renderer_create();
void cube_renderer_free(CubeRenderer *renderer);

void cube_renderer_draw(CubeRenderer *renderer, Cube *cube);

#endif