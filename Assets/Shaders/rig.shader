#shader vertex
#version 330 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = projMatrix * viewMatrix * position;
};

#shader fragment
#version 330 core

uniform vec3 u_Color;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(u_Color, 1);
};
