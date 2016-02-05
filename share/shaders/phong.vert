#version 120

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 mapping;

varying vec3 vnormal;
varying vec2 vmapping;

void main() {
    mat4 transform = projection * view * model;
    gl_Position = transform * vec4(position, 1.0);

    vnormal = mat3(model) * normal;
    vmapping = mapping;
}
