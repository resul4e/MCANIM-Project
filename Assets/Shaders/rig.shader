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

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5, 1, 1, 1);
};
