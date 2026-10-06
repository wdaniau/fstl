#version 300 es
precision mediump float;
in vec2 tex_coord;
uniform sampler2D label_texture;
out vec4 fragColor;
void main() { fragColor = texture(label_texture, tex_coord); }
