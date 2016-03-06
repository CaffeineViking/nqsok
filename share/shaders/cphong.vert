#version 120

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 position;
attribute vec3 normal;

varying vec4 vposition;
varying vec3 vnormal;

void main() {
    vec4 world_coordinates = model * vec4(position, 1.0);
    vec4 view_coordinates = view * world_coordinates;
    vec4 projection_coordinates = projection * view_coordinates;
    gl_Position = projection_coordinates;

    vposition = world_coordinates;
    vnormal = mat3(model) * normal;
}
