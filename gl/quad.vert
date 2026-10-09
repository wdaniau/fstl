
in vec2 vertex_position;
in vec2 vertex_uv;

out vec2 v_uv;

void main() {
    gl_Position = vec4(vertex_position, 0.0, 1.0);
    v_uv = vertex_uv;
}
