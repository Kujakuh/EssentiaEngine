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
	//std::vector<GameObject> ents = myEntity.entity->GetChildren();
	//std::cout << ents.size() << ents.at(0)->GetName() << std::endl;

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

	//Essentia::bindlessTexturesMode = false;
	//Essentia::render_mode = PBR;

	std::vector<std::string> faces
	{
			RESOURCES_PATH "Textures/right.jpg",
			RESOURCES_PATH "Textures/left.jpg",
			RESOURCES_PATH "Textures/top.jpg",
			RESOURCES_PATH "Textures/bottom.jpg",
			RESOURCES_PATH "Textures/front.jpg",
			RESOURCES_PATH "Textures/back.jpg"
	};

	CameraPerspective camera("Camera", scene, 45.0f, (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
	//CameraOrtho camera("Camera", scene, -10.0f * (_WIDTH / _HEIGHT) / 2.0f, 10.0f * (_WIDTH / _HEIGHT) / 2.0f, -10.0f / 2.0f, 10.0f / 2.0f, -0.1f, 0.1f);
	//Camera2D camera("Camera", scene, 90.0f, static_cast<float>(_WIDTH) / _HEIGHT, 0.1f, 100.0f);
	scene->RegisterSystems(Renderer2D(&camera), Renderer3D(&camera));

	ShaderLab f;
	Shader s(f.generateShader3D(VERTEX).c_str(), f.generateShader3D(FRAGMENT).c_str(), DATA_SOURCE::STR_DATA);
	Shader cube(RESOURCES_PATH "Shaders/cubemap.vert", RESOURCES_PATH "Shaders/cubemap.frag", FILE_PATH);

	s.use();
	TextureHandle gg = TextureManager::getTexture(RESOURCES_PATH "Textures/container.png", GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters3D);
	Material mat1; mat1.diffuse = gg;
	Mesh mesh(
		std::make_shared<Shader>(s),
		Essentia::cubeVertices, 
		Essentia::cubeIndices,
		{ mat1 }
	);
	mesh.shader->setUniform("projection", camera.getProjectionMatrix());
	s.disable();

	cube.use();
	TextureHandle tx = TextureManager::getTexture(RESOURCES_PATH "Textures/test.hdr", GL_TEXTURE_CUBE_MAP, TEX_TYPE::TEX_CUBEMAP);
	Material mat2; mat2.diffuse = tx;
	Mesh cubemap(
		std::make_shared<Shader>(cube),
		Essentia::cubeVertices,
		Essentia::cubeIndices,
		{ mat2 }
	);
	cubemap.shader->setUniform("projection", camera.getProjectionMatrix());
	cube.disable();

	camera.sensitivity = 0.05f;
	ref->setPosition().x += 1.5f;
	ref->updateMatrix();

	entity6->AddComponent<Sprite>(RESOURCES_PATH "Textures/mario.png");

	entity6->GetComponent<Transform>()->setScale().x = 3;
	entity6->GetComponent<Transform>()->setScale().y = 3;
	entity6->GetComponent<Transform>()->setPosition().x = 4;

	bool wireframeMode = false;
	dir direction = down;

	const char* modelo1 = RESOURCES_PATH "Models/lamp/Chandelier_03_4k.fbx";
	const char* modelo2 = RESOURCES_PATH "Models/lamp/Lantern_01_4k.fbx";
	const char* modelo3 = RESOURCES_PATH "Models/debug/brass_diya_lantern_4k.fbx";

	entity4->AddComponent<Model>();
	Model* mod = entity4->GetComponent<Model>();
	mod->loadModel(modelo3);
	mod->loadModel(modelo2);
	

	//entity4->GetComponent<Transform>()->setPosition().x += 5;
	//entity4->GetComponent<Transform>()->setPosition().z -= 12;
	entity4->GetComponent<Transform>()->setScale(Vector3(6.0f));
	entity4->GetComponent<Transform>()->rotate(Vector3(-90,0,0));

	while (!glfwWindowShouldClose(window))
	{
		InputManager::GetActiveInstance()->Update();
		Time::update();

		glfwSetWindowTitle(window, ("FPS: " +std::to_string(Time::getFps())).c_str());
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (InputManager::IsKeyPressed(KEY_SPACE))
		{
			ref->rotate(Vector3(0.024f * (float)glfwGetTime(), 0.015f * (float)glfwGetTime(), 0.03 * (float)glfwGetTime()));
			ref->updateMatrix();
		}
		if (InputManager::IsMouseButtonPressed(MOUSE_BTN_MIDDLE))
		{
			entity4->GetComponent<Transform>()->rotate(Vector3(0.14f, 0.45f, 0.2));
			entity4->GetComponent<Transform>()->updateMatrix();
		}
		if (InputManager::IsMouseButtonPressed(MOUSE_BTN_LEFT)) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		mod->getShader()->use();;
		mod->getShader()->setUniform("lightsNum", 0);
		mod->getShader()->setUniform("lights[0].position", camera.getPosition());
		mod->getShader()->setUniform("lights[0].type", 0);
		mod->getShader()->setUniform("lights[0].color", Vector3(1, 0.9, 0.8));
		mod->getShader()->setUniform("lights[0].intensity", 1.0f);
		mod->getShader()->disable();

		mesh.shader->use();
		mesh.shader->setUniform("lightsNum", 0);
		mesh.shader->setUniform("lights[0].position", camera.getPosition());
		mesh.shader->setUniform("lights[0].type", 0);
		mesh.shader->setUniform("lights[0].color", Vector3(1, 0.9, 0.8));
		mesh.shader->setUniform("lights[0].intensity", 1.0f);
		mesh.shader->disable();

		glDepthFunc(GL_LEQUAL);
		cubemap.initShader();
		cubemap.shader->setUniform("view", Matrix4(Matrix3(camera.getViewMatrix())));
		cubemap.render();
		cubemap.disableShader();
		glDepthFunc(GL_LESS);

		mesh.initShader();
		ref->updateMatrix();
		mesh.shader->setUniform("model", ref->getModelMatrix());
		mesh.shader->setUniform("view", camera.getViewMatrix());
		mesh.shader->setUniform("viewPos", camera.getPosition());
		mesh.render();
		mesh.disableShader();

		if (InputManager::IsKeyPressed(KEY_A)) camera.transform->setPosition() -= camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_D)) camera.transform->setPosition() += camera.getRight() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_S)) camera.transform->setPosition() -= camera.getFront() * camera.sensitivity;
		if (InputManager::IsKeyPressed(KEY_W)) camera.transform->setPosition() += camera.getFront() * camera.sensitivity;

		if (InputManager::IsKeyPressed(KEY_UP))
		{
			ref->setPosition().y += 0.1;
			//entity6->GetComponent<Transform>()->setPosition().y += 0.1;
			if (direction != up)
			{
				direction = up;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/mario.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_DOWN))
		{
			//entity6->GetComponent<Transform>()->setPosition().y -= 0.1;
			ref->setPosition().y -= 0.1;
			if (direction != down)
			{
				direction = down;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/mario.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_LEFT))
		{
			//entity6->GetComponent<Transform>()->setPosition().x -= 0.1;
			ref->setPosition().x -= 0.1;

			if (direction != left)
			{
				direction = left;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/left.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_RIGHT))
		{
			ref->setPosition().x += 0.1;
			//entity6->GetComponent<Transform>()->setPosition().x += 0.1;
			if (direction != right)
			{
				direction = right;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/right.png");
				std::cout << "15 secs Timer Started\n";
				Time::addTimer(std::make_shared<Timer>([]() {std::cout << "15 secs have passed \n";}, 15));
			}
		}
		if (InputManager::IsKeyPressed(KEY_2))
			mod->loadModel(modelo2);

		if (InputManager::IsKeyPressed(KEY_1) && ModelCacheManager::getInstance().isLoaded(modelo3))
			mod->loadModel(modelo3);
			
		camera.processMouseMovement(-InputManager::GetMouseData().x, InputManager::GetMouseData().y);

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