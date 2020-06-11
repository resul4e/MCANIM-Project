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

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

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

void main()
{
	vec3 N = v_Normal;
	vec3 L = normalize(lightPos - v_Position);
	vec3 texColor = toLinear(texture(u_Texture, v_TexCoord).rgb);
	vec3 Radiance = texColor * max(0, dot(N, L)) * lightIntensity;
	color = vec4(reinhardToneMapping(Radiance, 1.5), 1);
};
