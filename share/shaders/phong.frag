#version 120

varying vec3 vnormal;
varying vec2 vmapping;

void main() {
    const vec3 ambient_color = vec3(1.0);
    vec3 ambient = 0.2 * ambient_color;

    const vec3 light = vec3(0.58, 0.58, 0.58);
    const vec3 diffuse_color = vec3(1.0);
    float diffuse_angle = dot(normalize(light), normalize(vnormal));
    vec3 diffuse = 0.8 * diffuse_color * max(diffuse_angle, 0.0);

    const vec3 view = vec3(0.0, 0.0, -1.0);
    const vec3 specular_color = vec3(1.0); const int specular_shininess = 5;
    vec3 light_reflection = normalize(reflect(light, normalize(vnormal)));
    float specular_angle = dot(light_reflection, normalize(view));
    vec3 specular = 0.5 * specular_color * pow(max(specular_angle, 0.0), specular_shininess);
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
