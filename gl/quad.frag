#version 120
varying vec2 v_uv;

uniform vec3 colorBL;
uniform vec3 colorBR;
uniform vec3 colorTL;
uniform vec3 colorTR;

void main() {
    vec3 bottom = mix(colorBL, colorBR, v_uv.x);
    vec3 top = mix(colorTL, colorTR, v_uv.x);
    vec3 color = mix(bottom, top, v_uv.y);
    gl_FragColor = vec4(color, 1.0);
}