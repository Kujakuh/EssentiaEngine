#pragma once
#include <glad/glad.h>

struct Shader
{
	GLuint id = 0;

	bool ProgramFString(const char *vertexShaderData, const char *fragmentShaderData);
	bool ProgramFString(const char *vertexShaderData,
		const char *geometryShaderData, const char *fragmentShaderData);

	bool ProgramFFile(const char *vertexShader, const char *fragmentShader);
	bool ProgramFFile(const char *vertexShader,
		const char *geometryShader, const char *fragmentShader);


	void bind();

	void clear();

	GLint getUniform(const char *name);
};

GLint getUniform(GLuint shaderId, const char *name);
