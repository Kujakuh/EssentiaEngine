#include <Core/Graphics/Shader.hpp>

namespace Essentia
{
	// Utils -----------------------------------------------------------------------
#pragma region "Shader construct utility functions"

	void Shader::compileShader(GLuint shader, const char* shaderCode, SH_TYPE shaderType)
	{
		int compilationFlag;
		char* infoLog = new char[512]();

		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		// Check if compilation of the shader was successful :: snippet (Always the same)
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationFlag);
		if (!compilationFlag)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << " ERROR::SHADER::";
			std::cout << ST_TOSTRING[shaderType];
			std::cout << "::COMPILATION::FAILED\n" << infoLog << '\n';
			delete[] infoLog;
		}
	}

	void Shader::buildShaderFile(GLuint shader, const char* file_path, SH_TYPE shaderType)
	{
		std::string shader_code = readShaderFile(file_path);
		const char* s_shader_code = shader_code.c_str();

		compileShader(shader, s_shader_code, shaderType);
	}

	void Shader::linkShaderErrorCheck(GLuint shaderProgram)
	{
		int linkFlag;
		char* infoLog = new char[512]();

		// Check linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkFlag);
		if (!linkFlag)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED\n" << infoLog << '\n';
			delete[] infoLog;
		}
	}

	std::string Shader::readShaderFile(const char* shaderPath)
	{
		std::string shaderCode;
		std::ifstream shaderFile;
		std::stringstream shaderStream;

		// Ensure ifstream objects can throw exceptions
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// Open files
			shaderFile.open(shaderPath);

			// Read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();

			// Close file handlers
			shaderFile.close();

			// Convert stream into string
			shaderCode = shaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE::READ::FAILED" << '\n';
		}

		return shaderCode;
	}

#pragma endregion
	// -----------------------------------------------------------------------------

	Shader::Shader(const char* vertexPath, const char* fragmentPath, DATA_SOURCE dataSource)
	{
		this->ID = 0;
		GLuint vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		if (dataSource == FILE_PATH)
		{
			buildShaderFile(vertexShader, vertexPath, VERTEX);
			buildShaderFile(fragmentShader, fragmentPath, FRAGMENT);
		}
		else if (dataSource == STR_DATA)
		{
			compileShader(vertexShader, vertexPath, VERTEX);
			compileShader(fragmentShader, fragmentPath, FRAGMENT);
		}

		// Shader program creation and linking
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexShader);
		glAttachShader(this->ID, fragmentShader);
		glLinkProgram(this->ID);

		// Check linking errors
		linkShaderErrorCheck(this->ID);

		// Free Memory 
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, DATA_SOURCE dataSource)
	{
		this->ID = 0;
		GLuint vertexShader, fragmentShader, geometryShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		if (dataSource == FILE_PATH)
		{
			buildShaderFile(vertexShader, vertexPath, VERTEX);
			buildShaderFile(fragmentShader, fragmentPath, FRAGMENT);
			buildShaderFile(geometryShader, geometryPath, GEOMETRY);
		}
		else if (dataSource == STR_DATA)
		{
			compileShader(vertexShader, vertexPath, VERTEX);
			compileShader(fragmentShader, fragmentPath, FRAGMENT);
			compileShader(geometryShader, geometryPath, GEOMETRY);
		}

		// Shader program creation and linking
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexShader);
		glAttachShader(this->ID, fragmentShader);
		glAttachShader(this->ID, geometryShader);
		glLinkProgram(this->ID);

		// Check linking errors
		linkShaderErrorCheck(this->ID);

		// Free Memory 
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);
	}

	Shader::~Shader()
	{
		glUseProgram(0);
		glDeleteProgram(this->ID);
	}

	inline GLuint Shader::getID() const { return this->ID; }

	void Shader::use() const
	{
		glUseProgram(this->ID);
	}
	void Shader::disable() const
	{
		glUseProgram(0);
	}

	/*	Uniform utility functions:
	*
	*	Get the uniform "location" (var it self) in the shader program to be able to edit
	*	Set the values of the uniform at the given location
	*/

	void Shader::setUniform(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
	}
	void Shader::setUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void Shader::setUniform(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void Shader::setUniform(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void Shader::setUniform(const std::string& name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3);
	}
	void Shader::setUniform(const std::string& name, float v1, float v2, float v3, float v4) const
	{
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3, v4);
	}
	void Shader::setUniform(const std::string& name, glm::vec3 value) const
	{
		glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
	}
	void Shader::setUniform(const std::string& name, glm::vec4 value) const
	{
		glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
	}
	void Shader::setUniform(const std::string& name, glm::mat3 value) const
	{
		glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	void Shader::setUniform(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::bindUBOToBindingPoint(const GLchar* uniformBufferObjectName, GLuint bindingPointIndex) const
	{
		unsigned int ubo_index = glGetUniformBlockIndex(this->ID, uniformBufferObjectName);
		glUniformBlockBinding(this->ID, ubo_index, bindingPointIndex);
	}
}