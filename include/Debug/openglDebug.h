#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <glad/glad.h>

void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char *message,
	const void *userParam);

GLenum glCheckError_(const char* file, int line);

#endif // !DEBUG_UTILS_H