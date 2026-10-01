#version 300 es
precision highp float;
precision highp int;

in vec3 frag_color;

out vec4 fragColor;

void main() {
    fragColor = vec4(frag_color, 1.0);
}
