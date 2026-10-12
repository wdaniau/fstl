
in vec3 bary_pos;
uniform float wireWidth;

out vec4 fragColor;

void main() {
    vec3 d = bary_pos / fwidth(bary_pos);
    float minD = min(min(d.x, d.y), d.z);

    float alpha = 1.0 - smoothstep(wireWidth - 0.5, wireWidth + 0.5, minD);

    if (alpha < 0.01) discard;

    fragColor = vec4(1.0, 1.0, 1.0, alpha);
}

