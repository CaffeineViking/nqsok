#version 120

varying vec3 vnormal;
varying vec2 vmapping;

void main() {
    const vec3 color = vec3(1.0);
    const vec3 light = vec3(0.58, 0.58, 0.58);
    vec3 diffuse = color * max(dot(light, normalize(vnormal)), 0.0);
    gl_FragColor = vec4(diffuse, 1.0);
}
