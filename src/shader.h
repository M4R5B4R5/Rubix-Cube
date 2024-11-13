#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct shader {
    unsigned int program;
} Shader;

Shader *shader_create(char *vertex_path, char *fragment_path);
void shader_free(Shader *shader);

void shader_bind(Shader *shader);
void shader_setUniformMat4(Shader *shader, mat4 matrix, char *name);

#endif