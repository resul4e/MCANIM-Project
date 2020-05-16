#include <fstream>
#include <iostream>
#include <sstream>

#include "Shader.h"
#include <glad/glad.h>

Shader::Shader(std::filesystem::path path)
{
	ShaderProgramSource source = ParseShader(path);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertext" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}
ShaderProgramSource Shader::ParseShader(std::filesystem::path path)
{
	std::ifstream stream(path.string());

	if (stream.is_open() == true)
		std::cout << "SHADER FILE FOUND!" << std::endl;
	else
		std::cout << "SHADER FILE NOT FOUND!" << std::endl;

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream shaderstream[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			shaderstream[(int)type] << line << '\n';
		}

	}

	return{ shaderstream[0].str(),shaderstream[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

//bind the shader program
void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}
//unbind the shader program
void Shader::UnBind() const
{
	glUseProgram(0);
}
//Set a float uniform variable in a shader
void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniforLocation(name), value);
}
//Set a int uniform variable in a shader
void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniforLocation(name), value);
}
//Set 4 float variables in an uniform variable in a shader
void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	glUniform4f(GetUniforLocation(name), f0, f1, f2, f3);
}
//Set 4x4 float matrix in an uniform variable in a shader
void Shader::SetMatrix4(const std::string& name, glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniforLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
//Set 4x4 float matrix in an uniform variable in a shader
void Shader::UniformMatrix4fv(const std::string& name, glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniforLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
//Set vector2 of floats in an uniform variable in a shader
void Shader::SetVec2(const std::string& name, glm::fvec2 vector2)
{
	glUniform2f(GetUniforLocation(name), vector2.x, vector2.y);
}
//Set vector3 of floats in an uniform variable in a shader
void Shader::SetVec3(const std::string& name, glm::fvec3 vector3)
{
	glUniform3f(GetUniforLocation(name), vector3.x, vector3.y, vector3.z);
}
int Shader::GetUniforLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int  location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << "doesn't exist" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

