#include "camera.h"

Camera *camera_create(vec3 pos) {
    Camera *camera = malloc(sizeof(struct camera));
    glm_vec3_copy(pos, camera->position);
    glm_vec3_copy((vec3) {0.0f, 0.0f, 1.0f}, camera->gaze);

    return camera;
}

void camera_free(Camera *camera);

void camera_updateViewMatrix(Camera *camera, Shader *shader) {
    vec3 eye;
    glm_vec3_add(camera->position, camera->gaze, eye);

    glm_lookat(eye, camera->position, (vec3) {0.0f, 1.0f, 0.0f}, camera->view);
    shader_setUniformMat4(shader, camera->view, "view");
}

void camera_updateProjectionMatrix(Camera *camera, Shader *shader, float aspect) {
    glm_perspective(45.0f, aspect, 0.1f, 100.0f, camera->projection);
    shader_setUniformMat4(shader, camera->projection, "projection");
}