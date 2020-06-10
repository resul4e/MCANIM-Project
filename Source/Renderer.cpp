#include "Renderer.h"

#include "Joint.h"
#include "ModelLoader.h" // Temp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glad/glad.h>

Renderer::Renderer() :
	camera(glm::radians(60.0f), 800.0f/800, 1.0f, 10000.0f),
	renderRig(true)
{

}

void Renderer::Initialize(std::filesystem::path _assetPath)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader(_assetPath.string() + "/shader.shader");
	rigShader = new Shader(_assetPath.string() + "/rig.shader");

	// Generate a VAO and VBO for the armature drawing
	glGenVertexArrays(1, &m_armatureVao);
	glBindVertexArray(m_armatureVao);

	glGenBuffers(1, &m_armaturePbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_armaturePbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Renderer::Update(Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderModel(scene);
	if(renderRig)
	{
		RenderRig(scene);
	}
}

void Renderer::SetupQuad()
{

	texture = new Texture("../Assets/image.png");

	float vertices[20];
	int indices[6];

	unsigned int VBO, VAO, EBO;

	vertices[0] = 0.5f;
	vertices[1] = 0.75f;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;
	vertices[4] = 1.0f;

	vertices[5] = 0.5f;
	vertices[6] = -0.75f;
	vertices[7] = 0.0f;
	vertices[8] = 1.0f;
	vertices[9] = -.0f;

	vertices[10] = -0.5f;
	vertices[11] = -0.75f;
	vertices[12] = 0.0f;
	vertices[13] = -.0f;
	vertices[14] = -.0f;

	vertices[15] = -0.5f;
	vertices[16] = 0.75f;
	vertices[17] = 0.0f;
	vertices[18] = 0.0f;
	vertices[19] = 1.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::ToggleRigRendering()
{
	renderRig = !renderRig;
}

void Renderer::RenderModel(Scene& scene)
{
	// Enable testing against z-buffer depth
	glEnable(GL_DEPTH_TEST);

	scene.GetModel().UpdateVertices(scene.GetRig());
	shader->Bind();

	glm::mat4 projMatrix(1);
	camera.loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	viewMatrix = glm::translate(viewMatrix, -scene.GetCamera().position);

	glm::mat4 modelMatrix(1);
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f));

	shader->SetMatrix4("projMatrix", projMatrix);
	shader->SetMatrix4("viewMatrix", viewMatrix);
	shader->SetMatrix4("modelMatrix", modelMatrix);

	scene.GetTexture().Bind(0);
	shader->SetUniform1i("u_Texture", 0);
	for (Mesh& mesh : scene.GetModel().meshes)
	{
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size() * 3);
	}

	shader->UnBind();
}

struct Line
{
	glm::vec4 start;
	glm::vec4 end;
};

void ComputeArmature(std::vector<Line>& lines, const Joint& joint)
{
	glm::mat4 transform = joint.GetGlobalTransform();
	glm::vec4 startPosition = transform * glm::vec4(0, 0, 0, 1);
	
	std::vector<std::shared_ptr<Joint>>& children = joint.GetChildren();
	for (auto child : children)
	{
		glm::mat4 childTransform = child->GetGlobalTransform();
		glm::vec4 endPosition = childTransform * glm::vec4(0, 0, 0, 1);
		lines.push_back(Line{ startPosition, endPosition });

		ComputeArmature(lines, *child);
	}
}

void Renderer::RenderRig(Scene& scene)
{
	// Draw without considering z-buffer depth
	glDisable(GL_DEPTH_TEST);

	rigShader->Bind();

	glm::mat4 projMatrix(1);
	camera.loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	viewMatrix = glm::translate(viewMatrix, -scene.GetCamera().position);
	
	rigShader->SetMatrix4("projMatrix", projMatrix);
	rigShader->SetMatrix4("viewMatrix", viewMatrix);

	// Compute the armature lines
	const Rig& rig = scene.GetRig();
	std::vector<Line> bones;
	ComputeArmature(bones, *rig.GetRootJoint());

	// Render the armature
	glBindVertexArray(m_armatureVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_armaturePbo);
	glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(Line), bones.data(), GL_STREAM_DRAW);
	glDrawArrays(GL_LINES, 0, bones.size() * 2);

	rigShader->UnBind();
}
