#version 120

varying vec3 vnormal;
varying vec2 vmapping;

void main() {
    const vec3 ambient_color = vec3(1.0);
    const vec3 diffuse_color = vec3(1.0);
    const vec3 light = vec3(0.58, 0.58, 0.58);
    vec3 ambient = 0.2 * ambient_color;
    vec3 diffuse = 0.8 * diffuse_color * max(dot(light, normalize(vnormal)), 0.0);
    gl_FragColor = vec4(ambient + diffuse, 1.0);
}
