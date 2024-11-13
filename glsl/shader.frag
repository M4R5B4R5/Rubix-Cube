#version 330 core

in vec3 fragColor;
out vec3 oFragColor;

void main() {
    oFragColor = fragColor;
}