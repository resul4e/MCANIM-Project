#shader vertex
#version 330 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * position;
    v_Position = (modelMatrix * position).xyz;
    v_Normal = normal;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

uniform sampler2D u_Texture;
uniform sampler2D u_specular;
uniform bool u_HasTexture;

uniform sampler2D u_EnvTexture;

uniform vec3 u_CamPos;

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

const float PI = 3.141592653589793;
const float PI_OVER_TWO = PI / 2.0;
const float TWO_PI = PI * 2.0;

const float ONE_OVER_PI = 1.0 / PI;
const float ONE_OVER_TWO_PI = 1.0 / TWO_PI;

vec3 lightPos = vec3(0, 400, 1500);
vec3 lightIntensity = vec3(3);

vec3 toLinear(vec3 gammaColor) {
    return pow(gammaColor, vec3(2.2));
}

vec3 reinhardToneMapping(vec3 color, float exposure)
{
    color *= exposure / ((color / exposure) + 1);
    return color;
}

vec2 toUV(vec3 dir) {
    float phi = atan(dir.z, dir.x) + PI;
    float theta = asin(-dir.y) + PI_OVER_TWO;
    return vec2(phi * ONE_OVER_TWO_PI, 1-theta * ONE_OVER_PI);
}

void main()
{
    vec3 N = v_Normal;
    vec3 L = normalize(lightPos - v_Position);
    vec3 V = normalize(u_CamPos - v_Position);
    vec3 R = reflect(-V, N);
    vec3 H = normalize(L + V);
    vec3 envColor = texture(u_EnvTexture, toUV(N)).rgb;

    vec3 diffuseColor = vec3(0.3, 0.3, 0.3);
    if (u_HasTexture)
        diffuseColor = toLinear(texture(u_Texture, v_TexCoord).rgb);

    vec3 specularColor = toLinear(texture(u_EnvTexture, toUV(R)).rgb);
    if (u_HasTexture)
        specularColor *= texture(u_specular, v_TexCoord).rgb;
    specularColor *= pow(max(0, dot(H, N)), 50) * 4;

    vec3 Radiance = (diffuseColor + specularColor)  * max(0, dot(N, L)) * lightIntensity;
    Radiance += envColor * 0.1;
    
    color = vec4(reinhardToneMapping(Radiance, 1.5), 1);
};
