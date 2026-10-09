
in vec3 vertex_position;
in vec3 bary_position;   // (1,0,0) (0,1,0) (0,0,1)

uniform mat4 transform_matrix;
uniform mat4 view_matrix;

out vec3 ec_pos;
out vec3 bary_pos;

void main() {
    gl_Position = view_matrix * transform_matrix * vec4(vertex_position, 1.0);
    ec_pos = gl_Position.xyz;
    bary_pos = bary_position;
}

