#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <cglm/cglm.h>

#include "shader.h"

typedef struct camera {
    mat4 view;
    mat4 projection;

    vec3 position;
    vec3 gaze;
} Camera;

Camera *camera_create(vec3 position);
void camera_free(Camera *camera);

void camera_updateViewMatrix(Camera *camera, Shader *shader);
void camera_updateProjectionMatrix(Camera *camera, Shader *shader, float aspect);

#endif