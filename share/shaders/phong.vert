#version 120

uniform mat4 model;

attribute vec3 position;

void main() {
    mat4 transform = model;
    gl_Position = transform * vec4(position, 1.0);
}
