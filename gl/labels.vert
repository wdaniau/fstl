#version 300 es
in vec2 corner_position;               // -0.5 .. 0.5
uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform vec3 label_position;           // label position
uniform vec2 label_size;               // (w = h * aspectTexture, h)
uniform float offset_y;                // text offset on Y
uniform float label_angle;             // rotation in radians
uniform float aspect_ratio;            // screen aspect ratio
out vec2 tex_coord;                    // texture coord
void main() {
    vec4 clip = view_matrix * transform_matrix * vec4(label_position, 1.0);

    vec2 p = corner_position * label_size + vec2(0.0, offset_y);
    float c = cos(label_angle), s = sin(label_angle);
    p = vec2(c * p.x - s * p.y, s * p.x + c * p.y);
    p.x /= aspect_ratio;

    clip.xy += p * clip.w;
    gl_Position = clip;
    tex_coord = vec2(corner_position.x + 0.5, 0.5 - corner_position.y);
}
