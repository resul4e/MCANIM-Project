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

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

float checker(vec2 uv, float repeats) 
{
  float cx = floor(repeats * uv.x);
  float cy = floor(repeats * uv.y); 
  float result = mod(cx + cy, 2.0);
  return sign(result);
}

void main()
{
    vec2 uv = v_TexCoord;
    float fog = min(1, length(uv * 2 - 1));
    uv *= 40;
    float a = sign(mod(floor(uv.x) + floor(uv.y), 2.0));
    float c = mix(1.0, 0.9, a);
    float f = mix(c, 0.9, fog);
    color = vec4(vec3(f), 1);
};
