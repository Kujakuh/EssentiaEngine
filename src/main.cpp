#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <EssentiaEngine>

#include "SceneTemplate.cpp"
#include "GameObjectTemplate.cpp"

constexpr int _WIDTH = 900;
constexpr int _HEIGHT = (int) (0.5625*_WIDTH);

#ifndef LIBRARY_EXPORTS
#	define DllExport __declspec(dllexport)
#	define DllImport __declspec(dllimport)
#endif

//extern "C"{}

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
enum dir {
	left, right, up, down
};
int main(void)
{
	Essentia::init();
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

	entity6->AddComponent<Transform>();
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

#pragma region BLEND
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#pragma endregion

	Essentia::bindlessTexturesMode = true;

	std::vector<std::string> faces
	{
			RESOURCES_PATH "Textures/right.jpg",
			RESOURCES_PATH "Textures/left.jpg",
			RESOURCES_PATH "Textures/top.jpg",
			RESOURCES_PATH "Textures/bottom.jpg",
			RESOURCES_PATH "Textures/front.jpg",
			RESOURCES_PATH "Textures/back.jpg"
	};

	Shader s(RESOURCES_PATH "Shaders/vertex.vert", RESOURCES_PATH "Shaders/fragment.frag", FILE_PATH);
	Shader cube(RESOURCES_PATH "Shaders/cubemap.vert", RESOURCES_PATH "Shaders/cubemap.frag", FILE_PATH);

	s.use();
	TextureHandle gg = TextureManager::getTexture(RESOURCES_PATH "Textures/box.png", GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF);
	Mesh mesh(
		std::make_shared<Shader>(s),
		Essentia::cubeVertices, 
		Essentia::cubeIndices,
		{
			{"container", gg},
			{"face", TextureManager::getTexture(RESOURCES_PATH "Textures/face.png", GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF)}
		}
	);
	s.disable();

	cube.use();
	TextureHandle tx = Essentia::TextureManager::getTexture(RESOURCES_PATH "Textures/test.hdr", GL_TEXTURE_CUBE_MAP, TEX_TYPE::TEX_CUBEMAP);
	Mesh cubemap(
		std::make_shared<Shader>(cube),
		Essentia::cubeVertices,
		Essentia::cubeIndices,
		{
			//{"skybox", TextureManager::getCubemapTexture(faces, GL_TEXTURE_CUBE_MAP, TEX_TYPE::TEX_CUBEMAP, Essentia::defaultFilters)}
			{"skybox", tx}
		}
	);
	cube.disable();

	//CameraPerspective camera("Camera", scene, 45.0f, (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
	//CameraOrtho camera("Camera", scene, -10.0f * (_WIDTH / _HEIGHT) / 2.0f, 10.0f * (_WIDTH / _HEIGHT) / 2.0f, -10.0f / 2.0f, 10.0f / 2.0f, -0.1f, 0.1f);
	Camera2D camera("Camera", scene, 90.0f, static_cast<float>(_WIDTH) / _HEIGHT, 0.1f, 100.0f);
	scene->RegisterSystems(Essentia::Renderer2D(&camera));

	cubemap.initShader();
	cubemap.shader->setUniform("projection", camera.getProjectionMatrix());
	cubemap.disableShader();

	mesh.initShader();
	mesh.shader->setUniform("projection", camera.getProjectionMatrix());
	mesh.disableShader();

	camera.sensitivity = 0.05f;
	ref->setScale(Vector3(5, 5, 1));
	ref->setPosition().x -= 3;
	ref->updateMatrix();

	entity6->AddComponent<Sprite>(RESOURCES_PATH "Textures/mario.png");

	//entity6->GetComponent<Sprite>()->addCustomShaderMainCode(FRAGMENT, "vec4 gear = vec4(2,4,1,3);");
	//entity6->GetComponent<Sprite>()->addCustomShaderMainCodeFromFile(FRAGMENT, RESOURCES_PATH "Shaders/test.glsl");
	//entity6->GetComponent<Sprite>()->addCustomShaderMainCode(FRAGMENT, "FragColor = vec4(1,0.2,0.5,0.6);");

	entity6->GetComponent<Transform>()->setScale().x = 3;
	entity6->GetComponent<Transform>()->setScale().y = 3;
	entity6->GetComponent<Transform>()->setPosition().x = 4;

	bool wireframeMode = false;
	dir direction = down;
	while ( !glfwWindowShouldClose(window) )
	{
		InputManager::GetActiveInstance()->Update();
		showFPS(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (InputManager::IsKeyPressed(KEY_SPACE))
		{
			ref->rotate(Vector3(0.024f * (float)glfwGetTime(), 0.015f * (float)glfwGetTime(), 0.03 * (float)glfwGetTime()));
			ref->updateMatrix();
		}
		if(InputManager::IsMouseButtonPressed(MOUSE_BTN_LEFT)) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glDepthFunc(GL_LEQUAL);
		cubemap.initShader();
		cubemap.shader->setUniform("view", Matrix4(Matrix3(camera.getViewMatrix())));
		cubemap.render();
		cubemap.disableShader();
		glDepthFunc(GL_LESS);
		
		mesh.initShader();
		mesh.shader->setUniform("model", ref->getModelMatrix());
		mesh.shader->setUniform("view", camera.getViewMatrix());
		mesh.shader->setUniform("time", (float) glfwGetTime());
		mesh.render();
		mesh.disableShader();

		if (InputManager::IsKeyPressed(KEY_A)) camera.transform->setPosition() -= camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_D)) camera.transform->setPosition() += camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_S)) camera.transform->setPosition() -= camera.getFront() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_W)) camera.transform->setPosition() += camera.getFront() * camera.sensitivity;

		if (InputManager::IsKeyPressed(KEY_UP))
		{
			entity6->GetComponent<Transform>()->setPosition().y += 0.1;
			if (direction != up)
			{
				direction = up;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/mario.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_DOWN)) 
		{
			entity6->GetComponent<Transform>()->setPosition().y -= 0.1;
			if (direction != down)
			{
				direction = down;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/mario.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_LEFT))
		{
			entity6->GetComponent<Transform>()->setPosition().x -= 0.1;
			if (direction != left)
			{
				direction = left;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/left.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_RIGHT))
		{
			entity6->GetComponent<Transform>()->setPosition().x += 0.1;
			if (direction != right)
			{
				direction = right;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/right.png");
			}
		}

		//camera.processMouseMovement(-InputManager::GetMouseData().x, InputManager::GetMouseData().y);

		scene->Update();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (InputManager::IsKeyPressed(KEY_ESCAPE)) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);;
		if (InputManager::IsKeyPressed(KEY_TAB)) 
		{
			wireframeMode = !wireframeMode;
			if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}