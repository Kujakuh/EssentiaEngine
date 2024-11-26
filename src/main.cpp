#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

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

	GameObject entity4 = scene->CreateEntity("Entity4");
	GameObjectTemplate myEntity(scene);
	GameObject entity6 = scene->CreateEntity("Entity6");

	myEntity.entity->AddChild(entity4);
	myEntity.entity->AddChild(entity6);

	Transform t(Vector3(1),
				Quaternion(1, 1, 1, 1),
				Vector3(1));

	entity6->AddComponent<Transform>(t);
	entity4->AddComponent<Transform>(Vector3(1,2,4), Quaternion(0.3,-0.9, 0, 1), Vector3(1,3,2));
	Transform *ref = myEntity.entity->GetComponent<Transform>();

	if (entity4->HasComponent<Transform>())
	{
		ref->setPosition().x = 1;

		ref->rotate(Vector3(40, 30, 0));
		Matrix4 mat = ref->getModelMatrix();
		printMatrix(mat);
		try {
			std::cout << entity4->GetID() << scene->GetEntityByID(1)->GetName() << std::endl;
		}
		catch (std::runtime_error e) { std::cout << e.what() << "\n"; }
		ref->updateMatrix();
		printMatrix(ref->getModelMatrix());

	}
	std::vector<GameObject> ents = myEntity.entity->GetChildren();
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
	InputManager::Initialize(window);
	InputManager::SetActiveInstance(window);


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
	// VSYNC
	glfwSwapInterval(1);

    glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma region ENABLE OUTPUT DEBUG

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

#pragma endregion

#pragma region ANTI-ALIASING (opengl) MSAA
	glEnable(GL_MULTISAMPLE);
#pragma endregion 

	ska::flat_hash_map<FILTERS, GLenum> filters;
	filters[FILTERS::MIN_F] = GL_LINEAR;
	filters[FILTERS::MAG_F] = GL_LINEAR;
	filters[FILTERS::WRAP_S] = GL_REPEAT;
	filters[FILTERS::WRAP_T] = GL_REPEAT;

	std::vector<Vertex> meshVertices =
	{
		// Front face
		Vertex(Vector3(-0.5f, -0.5f,  0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f,  0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f,  0.5f,  0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f,  0.5f,  0.5f), Vector2(0.0f, 1.0f)),

		// Back face
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f,  0.5f, -0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f,  0.5f, -0.5f), Vector2(0.0f, 1.0f)),

		// Left face
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(-0.5f, -0.5f,  0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(-0.5f,  0.5f,  0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f,  0.5f, -0.5f), Vector2(0.0f, 1.0f)),

		// Right face
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f,  0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f,  0.5f,  0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(0.5f,  0.5f, -0.5f), Vector2(0.0f, 1.0f)),

		// Bottom face
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f,  0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f, -0.5f,  0.5f), Vector2(0.0f, 1.0f)),

		// Top face
		Vertex(Vector3(-0.5f,  0.5f, -0.5f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f,  0.5f, -0.5f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f,  0.5f,  0.5f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f,  0.5f,  0.5f), Vector2(0.0f, 1.0f))
	};

	// Definir los índices
	std::vector<GLuint> indices =
	{
		// Front face
		0, 1, 2,  0, 2, 3,
		// Back face
		4, 5, 6,  4, 6, 7,
		// Left face
		8, 9, 10, 8, 10, 11,
		// Right face
		12, 13, 14, 12, 14, 15,
		// Bottom face
		16, 17, 18, 16, 18, 19,
		// Top face
		20, 21, 22, 20, 22, 23
	};


	Shader s(RESOURCES_PATH "Shaders/vertex.vert", RESOURCES_PATH "Shaders/fragment.frag", FILE_PATH);

	Mesh mesh(
		s, 
		meshVertices, 
		indices,
		{
			{"container", TextureManager::getTexture(RESOURCES_PATH "Textures/1.png", GL_TEXTURE_2D, filters, TEX_TYPE::TEX_DIFF)},
			{"face", TextureManager::getTexture(RESOURCES_PATH "Textures/2.png", GL_TEXTURE_2D, filters, TEX_TYPE::TEX_DIFF)}
		}
	);

	mesh.initShader();

	Camera3D camera("CAM", scene, 45.0f, (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
	//Camera2D camera("2D Camera", scene, -10.0f * (_WIDTH / _HEIGHT) / 2.0f, 10.0f * (_WIDTH / _HEIGHT) / 2.0f, -10.0f / 2.0f, 10.0f / 2.0f, -1.0f, 1.0f);

	mesh.shader.setUniform("projection", camera.getProjectionMatrix());

	camera.sensitivity = 0.05f;

	while ( !glfwWindowShouldClose(window) )
	{
		InputManager::GetActiveInstance()->Update();
		showFPS(window);

		scene->Update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (InputManager::IsKeyPressed(KEY_SPACE) || InputManager::IsMouseButtonPressed(MOUSE_BTN_LEFT))
		{
			ref->rotate(Vector3(0.024f * (float)glfwGetTime(), 0.015f * (float)glfwGetTime(), 0.03 * (float)glfwGetTime()));
			ref->updateMatrix();
		}

		mesh.shader.setUniform("model", ref->getModelMatrix());
		mesh.shader.setUniform("view", camera.getViewMatrix());

		mesh.shader.setUniform("time", (float) glfwGetTime());
		mesh.render();

		if (InputManager::IsKeyPressed(KEY_A)) camera.transform->setPosition() -= camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_D)) camera.transform->setPosition() += camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_S)) camera.transform->setPosition() -= camera.getFront() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_W)) camera.transform->setPosition() += camera.getFront() * camera.sensitivity;

		camera.processMouseMovement(-InputManager::GetMouseData().x, InputManager::GetMouseData().y);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (InputManager::IsKeyPressed(KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}