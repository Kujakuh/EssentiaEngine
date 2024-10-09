/*
*	SHADER_S.HPP
*
*	This class is used to read and compile shaders
*
*	The constructor will read and compile given shaders
*
*	The use() function will activate the shader
*
*	The set of setUniform functions will modify the value of the shader uniform
*	placed at the given location
*
 */

//#ifndef SHADER_S_H
//#define SHADER_S_H
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader
{
private:

	// Shader program ID
	GLuint ID;

public:

	// The constructor will read and compile both shaders
	Shader(const char* vertexPath, const char* fragmentPath, int stringMode);

	~Shader();

	inline GLuint getID() const;

	void use();
	void disable();

	void setUniform(const std::string& name, bool value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, unsigned int value) const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, float v1, float v2, float v3) const;
	void setUniform(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setUniform(const std::string& name, glm::vec3 value) const;
	void setUniform(const std::string& name, glm::vec4 value) const;
	void setUniform(const std::string& name, glm::mat3 value) const;
	void setUniform(const std::string& name, glm::mat4 value) const;


};

//#endif // !SHADERS_S_H