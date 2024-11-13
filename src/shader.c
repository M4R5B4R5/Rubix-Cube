#include "shader.h"

static char *file_src(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("File does not exist");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *src = malloc(file_size * sizeof(char) + 1);
    src[file_size] = '\0';

    fread(src, file_size, 1, file);
    return src;
}

Shader *shader_create(char *vertex_path, char *fragment_path) {
    Shader *shader = malloc(sizeof(struct shader));
    shader->program = glCreateProgram();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    char *vertex_src = file_src(vertex_path);
    char *fragment_src = file_src(fragment_path);
    glShaderSource(vertex_shader, 1, (const GLchar *const *)&vertex_src, NULL);
    glShaderSource(fragment_shader, 1, (const GLchar *const *)&fragment_src, NULL);
    free(vertex_src);
    free(fragment_src);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    glAttachShader(shader->program, vertex_shader);
    glAttachShader(shader->program, fragment_shader);

    glLinkProgram(shader->program);
    shader_bind(shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader;
}

void shader_free(Shader *shader) {
    glDeleteProgram(shader->program);
    free(shader);
}

void shader_bind(Shader *shader) {
    glUseProgram(shader->program);
}

void shader_setUniformMat4(Shader *shader, mat4 matrix, char *name) {
    unsigned int location = glGetUniformLocation(shader->program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float*)matrix);
}