#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/shader.hpp>
#include <iostream>

#include <EssentiaEngine>

#include "SceneTemplate.cpp"
#include "GameObjectTemplate.cpp"

constexpr int _WIDTH = 500;
constexpr int _HEIGHT = (int) (0.5625*_WIDTH);

#ifndef LIBRARY_EXPORTS
#	define DllExport __declspec(dllexport)
#	define DllImport __declspec(dllimport)
#endif

//extern "C"{}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);


int main(void)
{
	SceneManager* sceneManager = SceneManager::GetInstance();

	SceneTemplate *scene = new SceneTemplate();
	sceneManager->ChangeScene(scene);

	GameObject entity1 = scene->CreateEntity("Entity1");
	GameObject entity2 = scene->CreateEntity("Entity2");
	GameObject entity3 = scene->CreateEntity("Entity3");
	scene->DestroyEntity(entity3);
	scene->DestroyEntity(entity2);
	scene->DestroyEntity(entity1);
	//entity1->AddComponent<Transform>();
	GameObject entity4 = scene->CreateEntity("Entity4");
	GameObjectTemplate myEntity(scene);
	GameObject entity6 = scene->CreateEntity("Entity6");

	Transform t(Vector3(1),
				Quaternion(1, 1, 1, 1),
				Vector3(1));

	entity6->AddComponent<Transform>(t);
	entity4->AddComponent<Transform>(Vector3(1,2,4), Quaternion(0.3,-0.9, 0, 1), Vector3(1,3,2));
	Transform *ref = myEntity.entity->GetComponent<Transform>();

	if (entity4->HasComponent<Transform>())
	{
		ref->setPosition().x = 24;

		ref->rotate(Vector3(90, 0, 0));
		Matrix4 mat = ref->getModelMatrix();
		printMatrix(mat);
		try {
			std::cout << entity4->GetID() << scene->GetEntityByID(1)->GetName() << std::endl;
		}
		catch (std::runtime_error e) { std::cout << e.what() << "\n"; }
		ref->updateMatrix();
		printMatrix(ref->getModelMatrix());

	}
	std::vector<GameObject> ents = scene->GetEntitiesWith<Transform>();
	std::cout << ents.size() << ents.at(0)->GetName() << std::endl;


	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#pragma region ENABLE DEBUG CONTEXT

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#pragma endregion

	// ANTI-ALIASING (multisample  MSAA) 1 -> 4 sampling point per fragment
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow *window = window = glfwCreateWindow(_WIDTH, _HEIGHT, "Essentia", NULL, NULL);
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
	s.setUniform("a", Vector3(1.0f, 0.3f, 0.0f));

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