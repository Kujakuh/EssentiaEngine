#ifndef DEBUG_UTILS_S
#define DEBUG_UTILS_S

#include <Debug/openglDebug.hpp>

// --https://learnopengl.com/In-Practice/Debugging
// --https://github.com/KhronosGroup/glslang

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char *message,
	const void *userParam)
{
	// ignore non-significant error/warning codes
	if (	id == 131169 
		||	id == 131185 
		||	id == 131218 
		||	id == 131204
		||	id == 131222
		||	id == 131140 //dittering error
	) return;

	if (type == GL_DEBUG_TYPE_PERFORMANCE) return;


	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message;
	std::cout << "---------------" << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	};

}

bool isExtensionSupported(const char* extensionName)
{
	GLint numExtensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (GLint i = 0; i < numExtensions; ++i)
	{
		const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);

		if (strcmp(ext, extensionName) == 0)
			return true;
	}
	return false;
}

void DebugDrawSkeleton(const Essentia::AssimpNodeData* node, const glm::mat4& parentTransform,
	Essentia::Shader& debugShader, glm::vec3 color, glm::mat4 projection) {
	glm::mat4 nodeTransform = node->transformation;
	glm::mat4 globalTransform = parentTransform * nodeTransform;

	// Dibujar una línea desde el nodo padre hasta el nodo actual
	glm::vec3 parentPos = glm::vec3(parentTransform[3]);
	glm::vec3 currentPos = glm::vec3(globalTransform[3]);

	// Si no es el nodo raíz, dibujar una línea
	if (glm::length(parentPos - currentPos) > 0.001f) {
		// Código para dibujar una línea simple usando OpenGL
		// (Necesitarás un shader básico para esto)
		debugShader.use();
		debugShader.setUniform("color", color);
		debugShader.setUniform("viewProjection", projection);

		float vertices[] = {
			parentPos.x, parentPos.y, parentPos.z,
			currentPos.x, currentPos.y, currentPos.z
		};

		// Configurar VAO/VBO para dibujar la línea
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 0, 2);

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	// Dibujar un punto en la posición actual del hueso
	// (Similar al código anterior, pero usando GL_POINTS)

	// Llamar recursivamente para los nodos hijos
	for (int i = 0; i < node->childrenCount; i++) {
		DebugDrawSkeleton(&node->children[i], globalTransform, debugShader, color, projection);
	}
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif // !DEBUG_UTILS_S