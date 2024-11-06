#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Debug/openglDebug.h>
#include <Core/shader.hpp>

#include <iostream>
#include <coroutine>
#include <format>

constexpr int _WIDTH = 500;
constexpr int _HEIGHT = (int) (0.5625*_WIDTH);

#ifndef LIBRARY_EXPORTS
#	define DllExport __declspec(dllexport)
#	define DllImport __declspec(dllimport)
#endif

//extern "C"{}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//std::coroutine<int> generator() {
//	for (int i = 0; i < 5; ++i) {
//		co_yield I;
//	}
//}

int main(void)
{
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#pragma region ENABLE DEBUG CONTEXT

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#pragma endregion

	// ANTI-ALIASING (multisample  MSAA) 1 -> 4 sampling point per fragment
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow *window = window = glfwCreateWindow(_WIDTH, _HEIGHT, "OG-L Engine", NULL, NULL);
	if (window == NULL)
    {
        std::cout << " ERROR::GLFW::WINDOW::CREATE" << '\n';
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);


	// Define and init render window and rescaling
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Try load Glad for his own OS-specific pointers
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress) )
    {
        std::cout << "ERROR::GLAD::INIT" << '\n';
        glfwTerminate();
        return -1;
    }

	// -- Most of the times this is an unneeded call
	// -- since default framebuffer coords are correctly mapped to NDC
	// -- but its not the case for all devices
	// glViewport(0, 0, _WIDTH, _HEIGHT);

    glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

    glfwSetWindowAspectRatio(window, 16, 9);

#pragma region ENABLE OUTPUT DEBUG

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

#pragma endregion

#pragma region ANTI-ALIASING (opengl) MSAA
	glEnable(GL_MULTISAMPLE);
#pragma endregion 

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(LOC_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::string a = std::string(RESOURCES_PATH) + "Shaders/vertex.vert";

	Shader s(a.c_str(), RESOURCES_PATH "Shaders/fragment.frag", FILE_PATH);
	s.use();
	s.setUniform("a", glm::vec3(1.0f, 0.3f, 0.0f));

	while ( !glfwWindowShouldClose(window) )
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}