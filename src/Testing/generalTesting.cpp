#include <future>
#include <thread>

#include <EssentiaEngine>

#include "SceneTemplate.cpp"
#include "GameObjectTemplate.cpp"
//#include "../GameDemo/player.cpp"
//#include "../GameDemo/demoScene.cpp"


constexpr int _WIDTH = 900;
constexpr int _HEIGHT = (int)(0.5625 * _WIDTH);

enum dir {
	_left, _right, _up, _down
};

static void generalTesting()
{
	AppConfig config;

	ConfigLoader configLoader(config.setAspectRatio(16, 9)
		.setWindowSize(_WIDTH)
		.setWindowTitle("Essentia")
		.setMSAASamples(16)
		.enableDepthTest(true)
		.enableBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		.setGLVersion(4, 3)
		.setWireframeMode(false)
		.enableDebugMode(true));

	WindowContext context;
	context.window = configLoader.createWindow();
	context.windowId = 0;

	GLFWwindow* window2 = configLoader.createWindow();

	configLoader.initializeOpenGL(window2);
	configLoader.initializeOpenGL(context.window);

	Essentia::init();
	Essentia::initDefaultModels();
	Essentia::render_mode = RENDER_MODE::PBR;
	Essentia::bindlessTexturesMode = true;

	WindowManager* windowManager = WindowManager::GetInstance();
	windowManager->RegisterWindowContext(context);

	SceneTemplate* scene = new SceneTemplate();
	windowManager->ChangeScene(context.windowId, scene);

	InputManager::Initialize(context.window);
	InputManager::SetActiveInstance(context.window);

	GameObject entity1 = scene->CreateEntity("Entity1");
	GameObject entity2 = scene->CreateEntity("Entity2");
	GameObject entity3 = scene->CreateEntity("Entity3");

	GameObject entity4 = scene->CreateEntity("Entity4");
	GameObjectTemplate myEntity(scene);
	GameObject entity6 = scene->CreateEntity("Entity6");

	Transform* ref = myEntity.entity->GetComponent<Transform>();

	EventSystem::emit(Essentia::InitEvent());

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

	std::vector<std::string> faces
	{
			RESOURCES_PATH "Textures/right.jpg",
			RESOURCES_PATH "Textures/left.jpg",
			RESOURCES_PATH "Textures/top.jpg",
			RESOURCES_PATH "Textures/bottom.jpg",
			RESOURCES_PATH "Textures/front.jpg",
			RESOURCES_PATH "Textures/back.jpg"
	};

	CameraPerspective camera("Camera", 45.0f, (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
	//CameraOrtho camera("Camera", scene, -10.0f * (_WIDTH / _HEIGHT) / 2.0f, 10.0f * (_WIDTH / _HEIGHT) / 2.0f, -10.0f / 2.0f, 10.0f / 2.0f, -0.1f, 0.1f);
	//Camera2D camera("Camera", scene, 45.0f, static_cast<float>(_WIDTH) / _HEIGHT, 0.1f, 100.0f);
	scene->RegisterSystems(Renderer2D(&camera), Renderer3D(&camera), AnimationController());

	//Skybox skybox(RESOURCES_PATH "Textures/test.hdr");
	Skybox skybox(faces);

	camera.sensitivity = 5.0f;
	ref->setPosition().x += 1.5f;
	ref->updateMatrix();

	entity6->AddComponent<Sprite>(RESOURCES_PATH "Textures/atlas2.png");
	Sprite* sprit = entity6->GetComponent<Sprite>();
	sprit->getTexture()->loadUVsFromJSON(RESOURCES_PATH "atlas2.json");
	//sprit->getTexture()->loadUVsFromJSON(RESOURCES_PATH "Textures/atlas2.png");

	sprit->useRegionFromAtlas("player_idle");

	entity6->GetComponent<Transform>()->setScale().x = 3;
	entity6->GetComponent<Transform>()->setScale().y = 3;
	entity6->GetComponent<Transform>()->setPosition().x = 4;

	bool wireframeMode = false;
	dir direction = _down;

	const char* modelo1 = RESOURCES_PATH "Models/lamp/Chandelier_03_4k.fbx";
	const char* modelo2 = RESOURCES_PATH "Models/lamp/Lantern_01_4k.fbx";
	const char* modelo3 = RESOURCES_PATH "Models/bones/rp_manuel_animated_001_dancing.fbx";
	const char* modelo4 = RESOURCES_PATH "Models/debug/wolf/Wolf-Blender-2.82a.gltf";

	// ---------------------------------- TESTING Y DEBUG ----------------------------------
	// -------------------------------------------------------------------------------------

	auto selectedModel = modelo2;
	entity4->GetComponent<Transform>()->setScale(Vector3(4.05f));
	entity1->AddComponent<LightSource>(LightType::Point);
	entity1->GetComponent<LightSource>()->SetIntensity(5.0f);

	// -------------------------------------------------------------------------------------
	// ---------------------------------- TESTING Y DEBUG ----------------------------------

	entity4->AddComponent<Model>(selectedModel);
	Model* mod = entity4->GetComponent<Model>();
	SkeletalAnimation testAnim(selectedModel, &mod->skeleton);
	SkeletalAnimation testAnim2(selectedModel, &mod->skeleton, 1);
	entity4->AddComponent<Animator>();
	entity4->GetComponent<Animator>()->AddState("idle", &testAnim);

	//mod->loadModel(modelo3);
	//mod->loadModel(modelo2);

	entity2->AddComponent<Model>(Essentia::sphere);
	entity2->GetComponent<Model>()->getMesh(0)->setAlbedo(TextureManager::getTexture(RESOURCES_PATH "Textures/container.png", GL_TEXTURE_2D, TEX_ALBEDO));
	entity2->GetComponent<Transform>()->setPosition().x += 6;

	//entity4->GetComponent<Transform>()->setPosition().x += 5;
	//entity4->GetComponent<Transform>()->setPosition().z -= 12;
	
	//entity4->GetComponent<Transform>()->rotate(Vector3(-90,0,0));

	std::string title;
	std::shared_ptr<Timer> timo = std::make_shared<Timer>(15, []() {std::cout << "Timer ended.\n";});
	int speed = 12;

	//entity2->active = false;

	int id_instance = scene->Instantiate(entity4, entity2->GetComponent<Transform>(), 10);
	GameObject test = scene->GetEntityByName(scene->GetInstances()[entity4->GetID()][0]);
	Transform* reference = test->GetComponent<Transform>();
	test->onUpdate = [test, reference]()
	{
		if (InputManager::IsKeyPressed(KEY_K))
			reference->setPosition().x += 0.002;
		if (InputManager::IsKeyPressed(KEY_L))
			reference->setPosition().x -= 0.002;
	};

	std::cout << scene->GetEntityByID(0)->GetName() << std::endl;

	SpriteKeyframe frame1("player_idle");
	SpriteKeyframe frame2("player_walk", 1.0f);
	SpriteAnimation anim(*sprit);
	anim.AddFrame(frame1);
	anim.AddFrame(frame2);

	testAnim.SetSpeed(0.5f);

	ShaderLab asp;
	Shader bonesShader(asp.generateShader3D(VERTEX).c_str(), asp.generateShader3D(FRAGMENT).c_str(), DATA_SOURCE::STR_DATA);

	//Player player(scene);

	//auto checkval = player.entity->GetComponent<Sprite>();

	while (!glfwWindowShouldClose(context.window))
	{
		auto dt = Time::deltaTime();

		InputManager::GetActiveInstance()->Update();
		Time::update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		title = "FPS: " + std::to_string(Time::fps());
		if (!timo->isDone()) title += " Timer runing, current time: " + std::to_string(timo->getElapsedTime());
		glfwSetWindowTitle(context.window, title.c_str());

		if (InputManager::IsKeyHeld(KEY_SPACE))
		{
			ref->rotate(Vector3(0.024f * (float)glfwGetTime(), 0.015f * (float)glfwGetTime(), 0.03 * (float)glfwGetTime()));
			ref->updateMatrix();
		}
		if (InputManager::IsMouseButtonHeld(MOUSE_BTN_MIDDLE))
		{
			entity4->GetComponent<Transform>()->rotate(Vector3(0.045f, 0.045f, 0), Vector3(2.0f,6.0f,2.0f));
			entity4->GetComponent<Transform>()->updateMatrix();
		}
		if (InputManager::IsMouseButtonPressed(MOUSE_BTN_LEFT)) glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		entity1->GetComponent<Transform>()->setPosition(camera.getPosition());
		entity1->GetComponent<LightSource>()->SetDirection(camera.getFront());

		skybox.render(camera.getProjectionMatrix(), Matrix4(Matrix3(camera.getViewMatrix())));

		if (InputManager::IsKeyHeld(KEY_A)) camera.transform->setPosition() -= camera.getRight() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyHeld(KEY_D)) camera.transform->setPosition() += camera.getRight() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyHeld(KEY_S)) camera.transform->setPosition() -= camera.getFront() * camera.sensitivity * Time::deltaTime();
		if (InputManager::IsKeyHeld(KEY_W)) camera.transform->setPosition() += camera.getFront() * camera.sensitivity * Time::deltaTime();

		if (InputManager::IsKeyHeld(KEY_UP))
		{
			//ref->setPosition().y += 0.1;
			entity6->GetComponent<Transform>()->setPosition().y += speed * Time::deltaTime();
			if (direction != _up)
			{
				direction = _up;
				//sprit->useRegionFromAtlas("player_idle");
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/right.png");
			}
		}
		if (InputManager::IsKeyHeld(KEY_LEFT))
		{
			entity6->GetComponent<Transform>()->setPosition().x -= speed * Time::deltaTime();
			//ref->setPosition().y -= 0.1;
			if (direction != _down)
			{
				direction = _down;
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/atlas2.png");
				sprit->useRegionFromAtlas("player_idle");
			}
		}
		if (InputManager::IsKeyHeld(KEY_DOWN))
		{
			//ref->setPosition().x -= 0.1;
			entity6->GetComponent<Transform>()->setPosition().y -= speed * Time::deltaTime();
			if (direction != _left)
			{
				direction = _left;
				//sprit->useRegionFromAtlas("player_walk");
				entity6->GetComponent<Sprite>()->setTexture(RESOURCES_PATH "Textures/left.png");
			}
		}
		if (InputManager::IsKeyHeld(KEY_RIGHT))
		{
			//ref->setPosition().x += 0.1;
			entity6->GetComponent<Transform>()->setPosition().x += speed * Time::deltaTime();
			if (direction != _right)
			{
				direction = _right;
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

		/*std::async(std::launch::async, [dt, &testAnim]() {
			testAnim.Update(dt);
		});*/

		if (InputManager::IsKeyPressed(KEY_2))
			entity4->active = !entity4->active;
		if (InputManager::IsKeyHeld(KEY_L))
			anim.Update(Time::deltaTime());
		if (InputManager::IsKeyPressed(KEY_0)) {
			if (testAnim.GetSpeed() > 0.0f)
				testAnim.Pause();
			else
				testAnim.Play();
		}

		//for (int i = 0; i < mod->skeleton.GetRootNode().childrenCount; i++) {
		//	DebugDrawSkeleton(&mod->skeleton.GetRootNode().children[i],
		//		glm::mat4(1.0f),
		//		bonesShader,
		//		glm::vec3(0,0,1), camera.getProjectionMatrix());
		//}

		camera.processMouseMovement(-InputManager::GetMouseData().x, InputManager::GetMouseData().y, 10);

		windowManager->GetCurrentScene(context.windowId)->Update();

		glfwSwapBuffers(context.window);
		glfwPollEvents();

		if (InputManager::IsKeyPressed(KEY_ESCAPE)) glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);;
		if (InputManager::IsKeyPressed(KEY_TAB))
		{
			wireframeMode = !wireframeMode;
			if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	glfwDestroyWindow(context.window);
}