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
	 glViewport(0, 0, _WIDTH, _HEIGHT);

    glEnable(GL_DEPTH_TEST);
	// VSYNC
	//glfwSwapInterval(1);

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

	Essentia::initDefaultModels();

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
	//Camera2D camera("Camera", scene, 45.0f, static_cast<float>(_WIDTH) / _HEIGHT, 0.1f, 100.0f);
	scene->RegisterSystems(Renderer2D(&camera), Renderer3D(&camera));

	Shader cube(RESOURCES_PATH "Shaders/cubemap.vert", RESOURCES_PATH "Shaders/cubemap.frag", FILE_PATH);

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

	camera.sensitivity = 5.0f;
	ref->setPosition().x += 1.5f;
	ref->updateMatrix();

	entity6->AddComponent<Sprite>(RESOURCES_PATH "Textures/atlas2.png");
	Sprite* sprit = entity6->GetComponent<Sprite>();
	sprit->getTexture()->loadUVsFromJSON(RESOURCES_PATH "atlas2.json");

	sprit->useRegionFromAtlas("player_idle");

	entity6->GetComponent<Transform>()->setScale().x = 3;
	entity6->GetComponent<Transform>()->setScale().y = 3;
	entity6->GetComponent<Transform>()->setPosition().x = 4;

	bool wireframeMode = false;
	dir direction = down;

	const char* modelo1 = RESOURCES_PATH "Models/lamp/Chandelier_03_4k.fbx";
	const char* modelo2 = RESOURCES_PATH "Models/lamp/Lantern_01_4k.fbx";
	const char* mmm = RESOURCES_PATH "Models/debug/scene.gltf";
	const char* modelo3 = RESOURCES_PATH "Models/debug/79477.fbx";

	entity4->AddComponent<Model>(modelo1);
	Model* mod = entity4->GetComponent<Model>();
	//mod->loadModel(modelo3);
	//mod->loadModel(modelo2);

	entity2->AddComponent<Model>(Essentia::cube);
	//entity2->GetComponent<Model>()->getMesh(0)->setAlbedo(TextureManager::getTexture(RESOURCES_PATH "Textures/container.png", GL_TEXTURE_2D, TEX_ALBEDO));
	entity2->GetComponent<Transform>()->setPosition().x += 6;

	//entity4->GetComponent<Transform>()->setPosition().x += 5;
	//entity4->GetComponent<Transform>()->setPosition().z -= 12;
	entity4->GetComponent<Transform>()->setScale(Vector3(1.3f));
	entity4->GetComponent<Transform>()->rotate(Vector3(-90,0,0));

	entity1->AddComponent<LightSource>(LightType::Point);
	entity1->GetComponent<LightSource>()->SetIntensity(1.0f);

	std::string title;
	std::shared_ptr<Timer> timo = std::make_shared<Timer>(15);
	int speed = 12;

	while (!glfwWindowShouldClose(window))
	{
		InputManager::GetActiveInstance()->Update();
		Time::update();

		title = "FPS: " + std::to_string(Time::fps());
		if (!timo->isDone()) title += " Timer runing, current time: " + std::to_string(timo->getElapsedTime());
		glfwSetWindowTitle(window, title.c_str());

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

		entity1->GetComponent<Transform>()->setPosition(camera.getPosition());
		entity1->GetComponent<LightSource>()->SetDirection(camera.getFront());

		glDepthFunc(GL_LEQUAL);
		cubemap.initShader();
		cubemap.shader->setUniform("view", Matrix4(Matrix3(camera.getViewMatrix())));
		cubemap.render();
		cubemap.disableShader();
		glDepthFunc(GL_LESS);

		if (InputManager::IsKeyPressed(KEY_A)) camera.transform->setPosition() -= camera.getRight() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyPressed(KEY_D)) camera.transform->setPosition() += camera.getRight() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyPressed(KEY_S)) camera.transform->setPosition() -= camera.getFront() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyPressed(KEY_W)) camera.transform->setPosition() += camera.getFront() * camera.sensitivity * Time::deltaTime();

		if (InputManager::IsKeyPressed(KEY_UP))
		{
			//ref->setPosition().y += 0.1;
			entity6->GetComponent<Transform>()->setPosition().y += speed * Time::deltaTime();
			if (direction != up)
			{
				direction = up;
				//sprit->useRegionFromAtlas("player_idle");
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/right.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_LEFT))
		{
			entity6->GetComponent<Transform>()->setPosition().x -= speed * Time::deltaTime();
			//ref->setPosition().y -= 0.1;
			if (direction != down)
			{
				direction = down;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/atlas2.png");
				sprit->useRegionFromAtlas("player_idle");
			}
		}
		if (InputManager::IsKeyPressed(KEY_DOWN))
		{
			//ref->setPosition().x -= 0.1;
			entity6->GetComponent<Transform>()->setPosition().y -= speed * Time::deltaTime();
			if (direction != left)
			{
				direction = left;
				//sprit->useRegionFromAtlas("player_walk");
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/left.png");
			}
		}
		if (InputManager::IsKeyPressed(KEY_RIGHT))
		{
			//ref->setPosition().x += 0.1;
			entity6->GetComponent<Transform>()->setPosition().x += speed * Time::deltaTime();
			if (direction != right)
			{
				direction = right;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/atlas2.png");
				sprit->useRegionFromAtlas("player_walk");
				std::cout << "15 secs Timer Started\n";
				Time::addTimer(timo);
			}
		}
		//if (InputManager::IsKeyPressed(KEY_2))
		//	mod->loadModel(modelo2);

		//if (InputManager::IsKeyPressed(KEY_1) && ModelCacheManager::getInstance().isLoaded(modelo3))
		//	mod->loadModel(modelo3);
			
		camera.processMouseMovement(-InputManager::GetMouseData().x, InputManager::GetMouseData().y, 10);

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