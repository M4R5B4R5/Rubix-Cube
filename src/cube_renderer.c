#include "cube_renderer.h"

#define CUBE_V 24
#define CUBE_FLOATS 72

static float cube_vertices[CUBE_FLOATS] = {
    // Front face
    -0.5f, -0.5f,  0.5f,   // Bottom-left
     0.5f, -0.5f,  0.5f,   // Bottom-right
     0.5f,  0.5f,  0.5f,   // Top-right
    -0.5f,  0.5f,  0.5f,   // Top-left

    // Back face
    -0.5f, -0.5f, -0.5f,   // Bottom-left
     0.5f, -0.5f, -0.5f,   // Bottom-right
     0.5f,  0.5f, -0.5f,   // Top-right
    -0.5f,  0.5f, -0.5f,   // Top-left

    // Left face
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // Right face
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,

    // Top face
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
};

static unsigned int cube_indices[] = {
    // Front face
    0, 1, 2,
    0, 2, 3,

    // Back face
    4, 5, 6,
    4, 6, 7,

    // Left face
    8, 9, 10,
    8, 10, 11,

    // Right face
    12, 13, 14,
    12, 14, 15,

    // Top face
    16, 17, 18,
    16, 18, 19,

    // Bottom face
    20, 21, 22,
    20, 22, 23
};

CubeRenderer *cube_renderer_create() {
    CubeRenderer *renderer = malloc(sizeof(struct cube_renderer));
    Shader *shader = shader_create("shader.vert", "shader.frag");
    renderer->shader = shader;

    glGenVertexArrays(1, &renderer->va);
    glBindVertexArray(renderer->va);
    
    glGenBuffers(1, &renderer->position_vb);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->position_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glGenBuffers(1, &renderer->color_vb);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->color_vb);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glGenBuffers(1, &renderer->eb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->eb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), (const void *)cube_indices, GL_STATIC_DRAW);

    return renderer;
}

void cube_renderer_free(CubeRenderer *renderer) {
    shader_free(renderer->shader);
    glDeleteVertexArrays(1, &renderer->va);
    glDeleteBuffers(1, &renderer->position_vb);
    glDeleteBuffers(1, &renderer->eb);
    free(renderer);
}

void cube_renderer_draw(CubeRenderer *renderer, Cube *cube) {
    shader_bind(renderer->shader);
    glBindVertexArray(renderer->va);

    float cube_colors[CUBE_FLOATS];
    cube_color_data(cube, cube_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_colors, GL_STATIC_DRAW);

    mat4 rotate;
    glm_mat4_identity(rotate);
    glm_rotate(rotate, cube->angle, cube->axis);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, cube->position);
    glm_scale(model, cube->dimensions);

    mat4 final;
    glm_mat4_mul(model, rotate, final);
    
    shader_setUniformMat4(renderer->shader, final, "model");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, sizeof(cube_indices), GL_UNSIGNED_INT, NULL);
}