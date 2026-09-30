
// uniform float zoom;

// in vec3 ec_pos;

// out vec4 fragColor;

// void main() {
//     fragColor = vec4(1.0, 1.0, 1.0, 1.0);
// }

in vec3 bary_pos;
uniform float wireWidth;

out vec4 fragColor;

void main() {
    vec3 d = bary_pos / fwidth(bary_pos);
    float minD = min(min(d.x, d.y), d.z);
    if (minD > wireWidth) discard;   // ne garder que les pixels proches d'une arête
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

