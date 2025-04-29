#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <Core/AnimationSys/BoneSys/AssimpNodeData.hpp>
#include <Core/Graphics/Shader.hpp>

void GLAPIENTRY glDebugOutput(	GLenum source,
								GLenum type,
								unsigned int id,
								GLenum severity,
								GLsizei length,
								const char *message,
								const void *userParam);

GLenum glCheckError_(const char* file, int line);
bool isExtensionSupported(const char* extensionName);
void DebugDrawSkeleton(const Essentia::AssimpNodeData* node, const glm::mat4& parentTransform,
	Essentia::Shader& debugShader, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), glm::mat4 projection = glm::mat4(1.0));
#endif // !DEBUG_UTILS_H