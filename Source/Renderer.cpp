#include "Renderer.h"

#include "Joint.h"
#include "ModelLoader.h" // Temp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <glad/glad.h>

Renderer::Renderer() :
	renderRig(true),
	skyTexture(0)
{

}

void Renderer::Initialize(std::filesystem::path _assetPath)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader(_assetPath.string() + "/Shaders/model.shader");
	planeShader = new Shader(_assetPath.string() + "/Shaders/plane.shader");
	rigShader = new Shader(_assetPath.string() + "/Shaders/rig.shader");
	skyShader = new Shader(_assetPath.string() + "/Shaders/sky.shader");

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

void Renderer::Resize(Scene& scene, unsigned int width, unsigned int height)
{
	scene.GetCamera().SetAspectRatio((float) width / height);
	renderWidth = width;
	renderHeight = height;
}

void Renderer::Update(Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, renderWidth, renderHeight);

	//RenderSky(scene);
	RenderGroundPlane(scene);
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
	scene.GetCamera().loadProjectionMatrix(projMatrix);

	glm::mat4 yawMatrix = glm::rotate(0.0f, glm::vec3(0, 1, 0));
	glm::mat4 pitchMatrix = glm::rotate(0.0f, glm::vec3(1, 0, 0));

	glm::mat4 viewMatrix(1);
	glm::vec3 modelCenter = scene.GetModel().m_bounds.getCenter();
	scene.GetCamera().LookAt(viewMatrix, scene.GetCamera().position, modelCenter, glm::vec3(0, 1, 0));

	glm::mat4 cameraBasis = glm::mat4(glm::mat3(viewMatrix));
	cameraBasis[3][3] = 1;
	//cameraBasis[2][2] = -cameraBasis[2][2];
	//cameraBasis = yawMatrix * pitchMatrix * cameraBasis;
	float pitch, yaw, z;
	glm::extractEulerAngleXYZ(cameraBasis, pitch, yaw, z);

	skyShader->SetUniform1i("tex", 0);
	skyShader->SetVec2("persp", glm::vec2(1.0f / projMatrix[0][0], 1.0f / projMatrix[1][1]));
	skyShader->SetMatrix4("cameraBasis", cameraBasis);
	glBindVertexArray(m_dummyVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::RenderGroundPlane(Scene& scene)
{
	// Enable testing against z-buffer depth
	glEnable(GL_DEPTH_TEST);

	planeShader->Bind();

	glm::mat4 projMatrix(1);
	scene.GetCamera().loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	glm::vec3 modelCenter = scene.GetModel().m_bounds.getCenter();

	scene.GetCamera().LookAt(viewMatrix, scene.GetCamera().position, modelCenter, glm::vec3(0, 1, 0));

	glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, scene.GetModel().m_bounds.getMin().y, 0));
	float maxDimension = scene.GetModel().m_bounds.getMaxDimension();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(maxDimension*8, 1, maxDimension*8));

	planeShader->SetMatrix4("projMatrix", projMatrix);
	planeShader->SetMatrix4("viewMatrix", viewMatrix);
	planeShader->SetMatrix4("modelMatrix", modelMatrix);

	planeShader->SetVec3("u_CamPos", scene.GetCamera().position);
	for (Mesh& mesh : scene.GetGroundPlane().meshes)
	{
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size() * 3);
	}

	planeShader->UnBind();
}
void Renderer::RenderModel(Scene& scene)
{
	// Enable testing against z-buffer depth
	glEnable(GL_DEPTH_TEST);

	shader->Bind();

	glm::mat4 projMatrix(1);
	scene.GetCamera().loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	glm::vec3 modelCenter = scene.GetModel().m_bounds.getCenter();
	scene.GetCamera().LookAt(viewMatrix, scene.GetCamera().position, modelCenter, glm::vec3(0, 1, 0));

	glm::mat4 modelMatrix(1);

	shader->SetMatrix4("projMatrix", projMatrix);
	shader->SetMatrix4("viewMatrix", viewMatrix);
	shader->SetMatrix4("modelMatrix", modelMatrix);

	if (scene.GetTexture() != nullptr)
	{
		scene.GetTexture()->Bind(0);
		shader->SetUniform1i("u_Texture", 0);
		shader->SetUniform1i("u_HasTexture", 1);
		scene.GetSpecularMap()->Bind(2);
		shader->SetUniform1i("u_specular", 2);
	}
	else
	{
		shader->SetUniform1i("u_HasTexture", 0);
	}
	
	skyTexture->Bind(1);
	shader->SetUniform1i("u_EnvTexture", 1);

	shader->SetVec3("u_CamPos", scene.GetCamera().position);
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
	scene.GetCamera().loadProjectionMatrix(projMatrix);

	glm::mat4 viewMatrix(1);
	glm::vec3 modelCenter = scene.GetModel().m_bounds.getCenter();
	scene.GetCamera().LookAt(viewMatrix, scene.GetCamera().position, modelCenter, glm::vec3(0, 1, 0));
	
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
	// Ignore the root bone drawing
	glDrawArrays(GL_LINES, 2, bones.size() * 2);

	rigShader->UnBind();
}
