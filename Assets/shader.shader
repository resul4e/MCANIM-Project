#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	mat4 projMatrix = mat4(
		1.73205080757, 0, 0, 0,
		0, 1.73205080757, 0, 0,
		0, 0, -1.002002002, -1,
		0, 0, -0.2002002002, 0
	);

	gl_Position = projMatrix * vec4(position + vec3(0, 0, -1), 1);
	v_Position = position * 0.003 + vec3(0, 0, -5);
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

vec3 lightPos = vec3(0, 5, 0);

void main()
{
	vec3 N = v_Normal;
	vec3 L = normalize(lightPos - v_Position);
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	float NdotL = max(0, dot(N, L));
	color = vec4(vec3(NdotL), 1);
};
