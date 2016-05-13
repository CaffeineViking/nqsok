#version 120

uniform sampler2D sampler;
varying vec2 vmapping;
varying vec4 vshading;
varying vec4 vcolor;

void main() {
    // See vertex shader for light calculations.
    vec4 texel = texture2D(sampler, vmapping);
    vec4 final_color = (texel * vcolor) * vshading;
    gl_FragColor = final_color; // Go, pipeline!
}
