#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

void GLAPIENTRY glDebugOutput(	GLenum source,
								GLenum type,
								unsigned int id,
								GLenum severity,
								GLsizei length,
								const char *message,
								const void *userParam);

GLenum glCheckError_(const char* file, int line);
void showFPS(GLFWwindow* window);
bool isExtensionSupported(const char* extensionName);

#endif // !DEBUG_UTILS_H