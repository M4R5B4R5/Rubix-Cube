#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "cube_renderer.h"
#include "camera.h"
#include "cube.h"
#include "rubix.h"

GLFWwindow *window;
Camera *camera;
CubeRenderer *renderer;

Rubix *rubix;

ptype plane_mode;
int direction = CLOCKWISE;

int wire_frame = 0;

void framebuffer_resize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Check if the key was pressed
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W) {
            if (wire_frame == 0) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wire_frame = 1;
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wire_frame = 0;
            }
        }

        if (key == GLFW_KEY_X) {
            glm_ivec3_copy(YZ_PLANE, plane_mode);
        } else if (key == GLFW_KEY_Y) {
            glm_ivec3_copy(XZ_PLANE, plane_mode);
        } else if (key == GLFW_KEY_Z) {
            glm_ivec3_copy(XY_PLANE, plane_mode);
        }

        // Check if the key is a number key (from '0' to '9')
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_3) {
            int local = (key - GLFW_KEY_0);
            if (RUBIX_SIZE % 2 == 0 && local > 0) {
                local += 1;
            }
            local -= (RUBIX_SIZE / 2) + 1;
            rubix_twist(rubix, plane_mode, local, direction);
        }

        if (key == GLFW_KEY_SPACE) {
            direction = -direction;
        }
    }
}

int main() {
    glfwInit();

    window = glfwCreateWindow(800.0f, 600.0f, "Rubix", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    glfwSetKeyCallback(window, key_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    renderer = cube_renderer_create();

    CubeRenderer *renderer = cube_renderer_create();
    camera = camera_create((vec3) {6.0f, 6.0f, 6.0f});
    rubix = rubix_create((vec3) {0.0f, 0.0f, 0.0f});

    glm_ivec3_copy(XY_PLANE, plane_mode);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.18f, 0.31f, 0.31f, 1.0f);
        
        mat4 rotate;
        glm_mat4_identity(rotate);
        glm_rotate(rotate, 0.02f, (vec3) {0.0f, 1.0f, 0.0f});

        glm_mat4_mulv3(rotate, camera->position, 1.0f, camera->position);

        vec3 target = {0.0f, 0.0f, 0.0f};

        vec3 direction;
        glm_vec3_sub(camera->position, target, direction);
        glm_normalize(direction);
        glm_vec3_copy(direction, camera->gaze);

        camera_updateViewMatrix(camera, renderer->shader);
        camera_updateProjectionMatrix(camera, renderer->shader, 800.0f/600.0f);

        // rotate_plane(plane, CLOCKWISE);
        rubix_render(rubix, renderer);  
        rubix_rotate(rubix);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    cube_renderer_free(renderer);
    rubix_free(rubix);

    glfwDestroyWindow(window);
    glfwTerminate();
}