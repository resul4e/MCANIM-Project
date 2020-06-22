#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"

#include <filesystem>
#include <memory>

class Renderer
{
public:
	Renderer();

	/**
	 * \brief Initializes the renderer by setting OpenGL state, loading the shaders
	 * and generating appropriate GPU objects.
	 * \param _assetPath The path that the assets are located at
	 */
	void Initialize(std::filesystem::path _assetPath);
	
	/**
	 * \brief Resizes the area on which rendering will happen
	 * \param _scene The scene containing the main camera for getting the aspect ratio
	 * \param _width The width of the area we want to render to
	 * \param _height The height of the area we want to render to
	 */
	void Resize(Scene& _scene, unsigned int _width, unsigned int _height);
	
	/**
	 * \brief Performs all rendering (model, rig, groundplane)
	 * \param _scene The scene containing the model, rig and groundplane
	 */
	void Update(Scene& _scene);
	
	/**
	 * \brief Sets a background image to use for sky rendering and environment lighting
	 * \param _texture The texture containing the cylindrical mapped environment image
	 */
	void SetBackgroundTexture(std::shared_ptr<Texture> _texture);

private:
	/**
	 * \brief Renders the background environment image rather than a flat color
	 * \param _scene The scene containing the model and camera
	 */
	void RenderSky(Scene& _scene);

	/**
	 * \brief Renders the ground checkerboard plane
	 * \param _scene The scene containing the model and camera
	 */
	void RenderGroundPlane(Model& _plane, Camera& _camera, float _size, float _height);

	/**
	 * \brief Renders the model geometry
	 * \param _scene The scene containing the model
	 */
	void RenderModel(Scene& _scene);

	/**
	 * \brief Renders all the bones of the model as a connected line hierarchy
	 * \param _scene The scene containing the armature
	 */
	void RenderRig(Scene& _scene);
	
	// Shaders
	Shader* m_modelShader;
	Shader* m_planeShader;
	Shader* m_rigShader;
	Shader* m_skyShader;

	// Environment texture
	std::shared_ptr<Texture> m_skyTexture;

	// Vertex array objects necessary for rendering
	unsigned int m_dummyVao;
	unsigned int m_armatureVao;
	unsigned int m_armaturePbo;

	// The screen size at which we should render
	int m_renderWidth;
	int m_renderHeight;
};
