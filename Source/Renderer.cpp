#include "Renderer.h"

#include "Joint.h"
#include "ModelLoader.h" // Temp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glad/glad.h>

Renderer::Renderer() :
	camera(glm::radians(60.0f), 800.0f/800, 1.0f, 10000.0f)
{

}

void Renderer::Initialize(std::filesystem::path _assetPath)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader(_assetPath.string() + "/shader.shader");
	m_bone = ModelLoader::LoadModel(_assetPath.string() + "/Bone.obj"); // Temp
	m_bone->Upload();
}

void Renderer::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderModel();
	RenderRig();
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

void Renderer::SetModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void Renderer::SetRig(std::shared_ptr<Rig> rig)
{
	m_rig = rig;
}

void Renderer::RenderModel()
{
	shader->Bind();

	glm::mat4 projMatrix(1);
	camera.loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -100, -300));

	glm::mat4 modelMatrix(1);
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f));

	shader->SetMatrix4("projMatrix", projMatrix);
	shader->SetMatrix4("viewMatrix", viewMatrix);
	shader->SetMatrix4("modelMatrix", modelMatrix);

	for (Mesh& mesh : m_model->meshes)
	{
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size() * 3);
	}

	shader->UnBind();
}

void Renderer::RenderRig()
{
	shader->Bind();

	glm::mat4 projMatrix(1);
	camera.loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -100, -300));
	
	shader->SetMatrix4("projMatrix", projMatrix);
	shader->SetMatrix4("viewMatrix", viewMatrix);

	for (auto& joint : m_rig->GetAllJoints())
	{
		glm::mat4 transform = glm::transpose(joint->GetGlobalTransform());
		transform = glm::scale(transform, glm::vec3(20.0f));
		shader->SetMatrix4("modelMatrix", transform);
		glBindVertexArray(m_bone->meshes[0].vao);
		glDrawArrays(GL_TRIANGLES, 0, m_bone->meshes[0].faces.size() * 3);
	}

	shader->UnBind();
}
