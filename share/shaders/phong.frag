#version 120

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

uniform Material material;

varying vec3 vnormal;
varying vec2 vmapping;

void main() {
    vec3 color = vec3(1.0);
    vec3 ambient = 0.2 * material.ambient;

    const vec3 light = vec3(0.58, 0.58, 0.58);
    float diffuse_angle = dot(normalize(light), normalize(vnormal));
    vec3 diffuse = 0.8 * material.diffuse * max(diffuse_angle, 0.0);

    const vec3 view = vec3(0.0, 0.0, -1.0);
    vec3 light_reflection = normalize(reflect(light, normalize(vnormal)));
    float specular_angle = dot(light_reflection, normalize(view));
    vec3 specular = 0.2 * material.specular * pow(max(specular_angle, 0.0), material.shininess);
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
