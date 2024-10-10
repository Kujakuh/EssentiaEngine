#pragma once
#include <glad/glad.h>

void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char *message,
	const void *userParam);

GLenum glCheckError_(const char* file, int line);