#shader vertex
#version 330 core

vec2 vertices[4] = vec2[] (
    vec2(-1, -1),
    vec2(1, -1),
    vec2(-1, 1),
    vec2(1, 1)
);

vec2 texCoords[4] = vec2[] (
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

out vec2 pass_texCoord;

void main() {
    vec2 position = vertices[gl_VertexID];
    pass_texCoord = texCoords[gl_VertexID];

    gl_Position = vec4(position, 0, 1);
}

#shader fragment

#version 330 core

uniform sampler2D tex;

uniform vec2 persp;
uniform mat4 cameraBasis;

in vec2 pass_texCoord;

out vec4 fragColor;

const float PI = 3.141592653589793;
const float PI_OVER_TWO = PI / 2.0;
const float TWO_PI = PI * 2.0;

const float ONE_OVER_PI = 1.0 / PI;
const float ONE_OVER_TWO_PI = 1.0 / TWO_PI;

vec3 toLinear(vec3 gammaColor) {
    return pow(gammaColor, vec3(2.2));
}

vec2 toUV(vec3 dir) {
    float phi = atan(dir.z, dir.x) + PI;
    float theta = asin(-dir.y) + PI_OVER_TWO;
    return vec2(phi * ONE_OVER_TWO_PI, 1-theta * ONE_OVER_PI);
}

void main()
{
    vec2 uv = pass_texCoord * 2 - 1;
    
    vec3 cx = cameraBasis[0].xyz;
    vec3 cy = cameraBasis[1].xyz;
    vec3 cz = cameraBasis[2].xyz;
    vec3 direction = normalize(persp.x * cx * uv.x + persp.y * cy * uv.y + cz);
    
    uv = toUV(direction);
    
    gl_FragDepth = 0.999;
    vec3 color = min(textureLod(tex, uv, 0).rgb, 1000);
    fragColor = vec4(color, 1);
}
