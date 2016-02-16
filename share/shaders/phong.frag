#version 120

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

struct Light {
    vec3 position;
    vec3 intensity;
    bool directional;
    bool enabled;
};

uniform Material material;
uniform Light lights[16];

varying vec4 vposition;
varying vec3 vnormal;
varying vec2 vmapping;

vec3 ambient(vec3 k, vec3 i) {
    vec3 blend = k * i;
    return blend;
}

vec3 diffuse(vec3 k, vec3 i,
             vec3 n, vec3 l) {
    vec3 blend = k * i;
    float reflectivity = max(dot(n, l), 0.0);
    return blend * reflectivity;
}

vec3 specular(vec3 k, vec3 i,
              vec3 r, vec3 v,
              int exponent) {
    vec3 blend = k * i;
    float reflectivity = max(dot(r, v), 0.0);
    float shininess = pow(reflectivity, exponent);
    return blend * shininess;
}

void main() {
    const vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 Iambi = ambient(material.ambient, light_color);

    const vec3 light_direction = normalize(vec3(0.58, 0.58, 0.58));
    vec3 Idiff = diffuse(material.diffuse, light_color,
                         normalize(vnormal), light_direction);

    const vec3 view = normalize(vec3(0.0, 0.0, 1.0));
    vec3 Ispec = specular(material.specular, light_color,
                          reflect(-light_direction, normalize(vnormal)),
                          view, material.shininess);
    vec3 I = Iambi + Idiff + Ispec;
    gl_FragColor = vec4(I, 1.0);
}
