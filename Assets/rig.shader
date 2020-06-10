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

vec3 lightPos = vec3(0, 250, 50);

void main()
{
	color = vec4(0.5, 1, 1, 1);
};