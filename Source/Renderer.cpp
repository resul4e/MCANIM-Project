#include "Renderer.h"

#include "Joint.h"
#include "ModelLoader.h" // Temp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glad/glad.h>

Renderer::Renderer() :
	camera(glm::radians(60.0f), 800.0f/800, 1.0f, 10000.0f),
	renderRig(true),
	skyTexture(0)
{

}

void Renderer::Initialize(std::filesystem::path _assetPath)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader(_assetPath.string() + "/shader.shader");
	rigShader = new Shader(_assetPath.string() + "/rig.shader");
	skyShader = new Shader(_assetPath.string() + "/sky.shader");

	// Generate a VAO and VBO for the armature drawing
	glGenVertexArrays(1, &m_armatureVao);
	glBindVertexArray(m_armatureVao);

	glGenBuffers(1, &m_armaturePbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_armaturePbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Set up dummy VAO for vertex-less rendering
	glGenVertexArrays(1, &m_dummyVao);
}

void Renderer::Update(Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderSky(scene);
	RenderModel(scene);
	RenderRig(scene);
}

void Renderer::SetBackgroundTexture(std::shared_ptr<Texture> texture)
{
	skyTexture = texture;
}

void Renderer::ToggleRigRendering()
{
	renderRig = !renderRig;
}

void Renderer::RenderSky(Scene& scene)
{
	if (skyTexture == nullptr) return;
	
	skyShader->Bind();
	skyTexture->Bind(0);

	glm::mat4 projMatrix(1);
	camera.loadProjectionMatrix(projMatrix);

	glm::mat4 yawMatrix = glm::rotate(0.0f, glm::vec3(0, 1, 0));
	glm::mat4 pitchMatrix = glm::rotate(0.0f, glm::vec3(1, 0, 0));

	glm::mat4 cameraBasis(1);
	cameraBasis[2][2] = -1;
	cameraBasis = yawMatrix * pitchMatrix * cameraBasis;

	skyShader->SetUniform1i("tex", 0);
	skyShader->SetVec2("persp", glm::vec2(1.0f / projMatrix[0][0], 1.0f / projMatrix[1][1]));
	skyShader->SetMatrix4("cameraBasis", cameraBasis);
	glBindVertexArray(m_dummyVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
	if (!renderRig) return;

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
