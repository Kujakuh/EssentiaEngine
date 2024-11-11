/*
*	SHADER.HPP
*
*	This class is used to read and compile shaders
*
*	Constructor will read and compile given shaders either
*	from a specified string or file path
*
*	use() and disable() methods
*
*	setUniform() will modify the value of the
*	current shader uniform that has the provided name
* 
*	(
*		Overload for glm vec3, vec4, mat3, mat4, lose floats,
*		single float, int, unsigned int, and bool.
*	)
*
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <Core/enums.hpp>
#include <Core/values.hpp>

class Shader
{
	private:

		// Shader program ID
		GLuint ID;

	public:

		// The constructor will read and compile both shaders
		Shader(const char* vertexPath, const char* fragmentPath, DATA_SOURCE dataSource);
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, DATA_SOURCE dataSource);

		~Shader();

		inline GLuint getID() const;

		void compileShader(GLuint shader, const char* shaderCode, SH_TYPE shaderType);
		void buildShaderFile(GLuint shader, const char* file_path, SH_TYPE shaderType);
		void linkShaderErrorCheck(GLuint shaderProgram);
		std::string readShaderFile(const char* shaderPath);

		void use() const;
		void disable() const;

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

		// Uniform buffer object : shared between shader programs uniform
		// Setup data with glBufferSubData()
		// LearnOpenGL pg: 235/523
		void bindUBOToBindingPoint(const GLchar* uniformBufferObjectName, GLuint bindingPointIndex);
};

#endif // !SHADERS_H