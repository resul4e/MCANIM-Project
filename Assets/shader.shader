#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform float numberOfRows;
uniform vec2 offset;

uniform mat4 orthographicModel;

void main()
{
	gl_Position = orthographicModel * vec4(position.x, position.y, position.z, 1);
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

uniform vec3 ambientLight;

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord) * new vec4(ambientLight, 1);
	color = texColor;
};