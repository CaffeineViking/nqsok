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
    int directional;
    int enabled;
};

uniform vec3 view_direction;
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
    const vec3 ambient_color = vec3(1.0, 1.0, 1.0);
    vec3 Iambi = ambient(material.ambient, ambient_color);

    vec3 Idiff = vec3(0.0),
         Ispec = vec3(0.0);
    vec3 normal = normalize(vnormal);
    for (int i = 0; i < 16; ++i) {
        vec3 light_vector = vec3(0.0),
             light_intensity = vec3(0.0);
        if (lights[i].enabled == 0) break;
        if (lights[i].directional == 1) {
            light_vector = normalize(lights[i].position);
            light_intensity = lights[i].intensity;
        } else if (lights[i].directional == 0) {
            light_vector = lights[i].position - vposition.xyz;
            light_intensity = lights[i].intensity / pow(length(light_vector), 2);
            light_vector = normalize(light_vector);
        }

        const vec3 view = normalize(vec3(0.0, 0.0, 1.0));
        vec3 view_normal = normalize(vec4(view_direction, 1.0) - vposition).xyz;
        Idiff += diffuse(material.diffuse, light_intensity, normal, light_vector);
        Ispec += specular(material.specular, light_intensity,
                            reflect(-light_vector, normal),
                            view_normal, material.shininess);
    }

    vec3 I = Iambi + Idiff + Ispec;
    gl_FragColor = vec4(I, 1.0);
}
