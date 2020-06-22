#include "Renderer.h"

#include "Joint.h"
#include "Options.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glad/glad.h>

bool Options::RenderModel = true;
bool Options::RenderTextures = true;
bool Options::RenderRig = true;

Renderer::Renderer() :
	m_modelShader(nullptr),
	m_planeShader(nullptr),
	m_rigShader(nullptr),
	m_skyShader(nullptr),
	m_skyTexture(nullptr),
	m_dummyVao(0),
	m_armatureVao(0),
	m_armaturePbo(0),
	m_renderWidth(0),
	m_renderHeight(0)
{

}

void Renderer::Initialize(std::filesystem::path _assetPath)
{
	// Set the background color to a light grey
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Load all the shaders from file
	m_modelShader = new Shader(_assetPath.string() + "/Shaders/model.shader");
	m_planeShader = new Shader(_assetPath.string() + "/Shaders/plane.shader");
	m_rigShader = new Shader(_assetPath.string() + "/Shaders/rig.shader");
	m_skyShader = new Shader(_assetPath.string() + "/Shaders/sky.shader");

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

void Renderer::Resize(Scene& _scene, unsigned int _width, unsigned int _height)
{
	// Adjust camera aspect ratio
	_scene.GetCamera().SetAspectRatio((float) _width / _height);
	// Store width and height for render viewport
	m_renderWidth = _width;
	m_renderHeight = _height;
}

void Renderer::Update(Scene& _scene)
{
	// Clear the background and set the viewport to the window size
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_renderWidth, m_renderHeight);

	// Render the checkerboard floor
	float planeSize = _scene.GetModel().m_bounds.getMaxDimension() * 8;
	float planeHeight = _scene.GetModel().m_bounds.getMin().y;
	RenderGroundPlane(_scene.GetGroundPlane(), _scene.GetCamera(), planeSize, planeHeight);

	// Render the model geometry
	RenderModel(_scene);

	// Render the model armature
	RenderRig(_scene);
}

void Renderer::SetBackgroundTexture(std::shared_ptr<Texture> _texture)
{
	m_skyTexture = _texture;
}

void Renderer::RenderSky(Scene& _scene)
{
	if (m_skyTexture == nullptr) return;
	
	m_skyShader->Bind();

	glm::mat4 projMatrix(1);
	_scene.GetCamera().loadProjectionMatrix(projMatrix);

	glm::mat4 yawMatrix = glm::rotate(0.0f, glm::vec3(0, 1, 0));
	glm::mat4 pitchMatrix = glm::rotate(0.0f, glm::vec3(1, 0, 0));

	glm::mat4 viewMatrix(1);
	glm::vec3 modelCenter = _scene.GetModel().m_bounds.getCenter();
	_scene.GetCamera().LookAt(viewMatrix, _scene.GetCamera().position, modelCenter, glm::vec3(0, 1, 0));

	// Find the camera basis vectors
	glm::mat4 cameraBasis = glm::mat4(glm::mat3(viewMatrix));
	cameraBasis[3][3] = 1;

	m_skyShader->SetUniform1i("tex", 0);
	m_skyShader->SetVec2("persp", glm::vec2(1.0f / projMatrix[0][0], 1.0f / projMatrix[1][1]));
	m_skyShader->SetMatrix4("cameraBasis", cameraBasis);
	glBindVertexArray(m_dummyVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::RenderGroundPlane(Model& _plane, Camera& _camera, float _size, float _height)
{
	// Enable testing against z-buffer depth
	glEnable(GL_DEPTH_TEST);

	m_planeShader->Bind();

	// Fill and upload PVM matrices
	glm::mat4 projMatrix(1);
	glm::mat4 viewMatrix(1);
	glm::mat4 modelMatrix(1);

	_camera.loadProjectionMatrix(projMatrix);
	_camera.LookAt(viewMatrix, _camera.position, _camera.center, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, _height, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1, _size));

	m_planeShader->SetMatrix4("projMatrix", projMatrix);
	m_planeShader->SetMatrix4("viewMatrix", viewMatrix);
	m_planeShader->SetMatrix4("modelMatrix", modelMatrix);

	// Render plane
	for (Mesh& mesh : _plane.m_meshes)
	{
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size() * 3);
	}

	m_planeShader->UnBind();
}

void Renderer::RenderModel(Scene& _scene)
{
	if (!Options::RenderModel) return;

	// Enable testing against z-buffer depth
	glEnable(GL_DEPTH_TEST);

	m_modelShader->Bind();

	// Fill and upload PVM matrices
	glm::mat4 projMatrix(1);
	glm::mat4 viewMatrix(1);
	glm::mat4 modelMatrix(1);

	_scene.GetCamera().loadProjectionMatrix(projMatrix);
	_scene.GetCamera().LookAt(viewMatrix, _scene.GetCamera().position, _scene.GetCamera().center, glm::vec3(0, 1, 0));

	m_modelShader->SetMatrix4("projMatrix", projMatrix);
	m_modelShader->SetMatrix4("viewMatrix", viewMatrix);
	m_modelShader->SetMatrix4("modelMatrix", modelMatrix);

	// Bind model textures
	if (_scene.GetTexture() != nullptr && Options::RenderTextures)
	{
		_scene.GetTexture()->Bind(0);
		m_modelShader->SetUniform1i("u_Texture", 0);
		m_modelShader->SetUniform1i("u_HasTexture", 1);
		_scene.GetSpecularMap()->Bind(2);
		m_modelShader->SetUniform1i("u_specular", 2);
	}
	else
	{
		m_modelShader->SetUniform1i("u_HasTexture", 0);
	}
	
	// Bind environment texture
	m_skyTexture->Bind(1);
	m_modelShader->SetUniform1i("u_EnvTexture", 1);
	// Set camera position
	m_modelShader->SetVec3("u_CamPos", _scene.GetCamera().position);

	// Render model
	for (Mesh& mesh : _scene.GetModel().m_meshes)
	{
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size() * 3);
	}

	m_modelShader->UnBind();
}

struct Line
{
	glm::vec4 start;
	glm::vec4 end;
};

// Compute the line segments that need to be drawn to represent the armature hierarchy
void ComputeArmature(std::vector<Line>& _lines, const Joint& _joint)
{
	glm::mat4 transform = _joint.GetGlobalTransform();
	glm::vec4 startPosition = transform * glm::vec4(0, 0, 0, 1);
	
	std::vector<std::shared_ptr<Joint>>& children = _joint.GetChildren();
	for (auto child : children)
	{
		glm::mat4 childTransform = child->GetGlobalTransform();
		glm::vec4 endPosition = childTransform * glm::vec4(0, 0, 0, 1);
		_lines.push_back(Line{ startPosition, endPosition });

		ComputeArmature(_lines, *child);
	}
}

void Renderer::RenderRig(Scene& _scene)
{
	if (!Options::RenderRig) return;

	// Draw without considering z-buffer depth
	glDisable(GL_DEPTH_TEST);
	m_rigShader->Bind();

	// Fill and upload PVM matrices
	glm::mat4 projMatrix(1);
	glm::mat4 viewMatrix(1);

	_scene.GetCamera().loadProjectionMatrix(projMatrix);
	_scene.GetCamera().LookAt(viewMatrix, _scene.GetCamera().position, _scene.GetCamera().center, glm::vec3(0, 1, 0));
	
	m_rigShader->SetMatrix4("projMatrix", projMatrix);
	m_rigShader->SetMatrix4("viewMatrix", viewMatrix);

	// Set armature rendering color
	m_rigShader->SetVec3("u_Color", Options::RenderModel ? glm::vec3(0.5, 1, 1) : glm::vec3(0, 0, 0));

	// Compute the armature lines
	const Rig& rig = *_scene.GetRig();
	std::vector<Line> bones;
	ComputeArmature(bones, *rig.GetRootJoint());

	// Render the armature
	glBindVertexArray(m_armatureVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_armaturePbo);
	glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(Line), bones.data(), GL_STREAM_DRAW);
	// Ignore the root bone drawing
	glDrawArrays(GL_LINES, 2, bones.size() * 2);

	m_rigShader->UnBind();
}
