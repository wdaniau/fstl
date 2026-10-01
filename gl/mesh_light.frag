#version 300 es
precision highp float;
precision highp int;

uniform float zoom;
uniform vec4 ambient_light_color;
uniform vec4 directive_light_color;
uniform vec3 directive_light_direction;
uniform bool useWire;
uniform vec3 wireColor;
uniform float wireWidth;

in vec3 ec_pos;
in vec3 bary_pos;

out vec4 fragColor;

void main() {
    vec3 dir = normalize(directive_light_direction);

    vec3 ec_normal = normalize(cross(dFdx(ec_pos), dFdy(ec_pos)));
    ec_normal.z *= zoom;
    ec_normal = normalize(ec_normal);

    vec3 color = ambient_light_color.w * ambient_light_color.xyz
               + directive_light_color.w * dot(ec_normal, dir) * directive_light_color.xyz;

    if (useWire) {
        vec3 d = bary_pos / fwidth(bary_pos);
        float minD = min(min(d.x, d.y), d.z);
        float mixVal = smoothstep(wireWidth-1.0, wireWidth+1.0, minD);
        color = mix(wireColor, color, mixVal);
    }

    fragColor = vec4(color, 1.0);
}
