#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>

#include "../../../ThirdParty/glm/include/glm/glm.hpp"
#include "../../../ThirdParty/glm/include/glm/gtc/type_ptr.inl"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(std::filesystem::path path);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniforms.
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3); // Vec4 float
	void SetUniform1f(const std::string& name, float value);                            // Set  float
	void SetUniform1i(const std::string& name, int value);                              // Set  int
	void SetMatrix4(const std::string& name, glm::mat4& matrix);                        // Set  mat4
	void UniformMatrix4fv(const std::string& name, glm::mat4& matrix);                  // Set  mat4 
	void SetVec2(const std::string& name, glm::fvec2 vector2);                          // Set  vec2 of floats
	void SetVec3(const std::string& name, glm::fvec3 vector3);                          // Set  vec3 of floats
	int  GetUniforLocation(const std::string& name);                                    // Gets the location of the uniform

private:
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(std::filesystem::path path);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
