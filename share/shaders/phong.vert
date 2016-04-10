#version 120
#define LIGHTS 16

struct Material {
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

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform Material material;
uniform Light lights[LIGHTS];
uniform vec3 camera_position;
uniform vec3 ambient_color;

attribute vec3 position;
attribute vec2 mapping;
attribute vec3 normal;
attribute vec3 color;

varying vec2 vmapping;
varying vec4 vshading;
varying vec4 vcolor;

vec3 ambient(vec3 k, vec3 i) {
    vec3 blend = k * i;
    return blend;
}

vec3 diffuse(vec3 k, vec3 i,
             vec3 n, vec3 l) {
    vec3 blend = k * i;
    float reflectivity = dot(n, l);
    reflectivity = max(reflectivity, 0.0);
    return blend * reflectivity;
}

vec3 specular(vec3 k, vec3 i,
              vec3 r, vec3 v,
              int      alpha) {
    vec3 blend = k * i;
    float reflectivity = dot(r, v);
    reflectivity = max(reflectivity, 0.0);
    float shininess = pow(reflectivity, alpha);
    return blend * shininess;
}

void main() {
    vec4 hposition = vec4(position, 1.0);
    vec4 world_coordinates = model * hposition;
    vec3 world_position = world_coordinates.xyz;
    vec4 view_coordinates = view * world_coordinates;
    vec4 projected_coordinates = projection * view_coordinates;
    gl_Position = projected_coordinates; // Do your magic OGL!

    vec3 view_normal = normalize(camera_position - world_position);
    vec3 iambi = ambient(material.diffuse, ambient_color);
    vec3 idiff = vec3(0.0), ispec = vec3(0.0);
    for (int n = 0; n < LIGHTS; ++n) {
        if (lights[n].enabled == 0) continue;
        vec3 light_vector, light_intensity;

        if (lights[n].directional == 1) {
            light_intensity = lights[n].intensity;
            light_vector = normalize(lights[n].position);
            // Since light is directional, speed -> very nice.
        } else if (lights[n].directional == 0) { // Calculate light from vertex.
            light_vector = lights[n].position - world_position; // Need distance.
            light_intensity = lights[n].intensity / pow(length(light_vector), 2);
            // Quadratic falloff is a better approximation than linear (citation?).
            light_vector = normalize(light_vector); // Waste of computation time...
        }

        idiff += diffuse(material.diffuse, light_intensity,
                         normal, light_vector);
        ispec += specular(material.specular, light_intensity,
                          reflect(-light_vector, normal), view_normal,
                          material.shininess);
    }

    vec3 isum = iambi + idiff + ispec;
    vshading = vec4(isum, 1.0);
    vcolor = vec4(color, 1.0);
    vmapping = mapping;
}
