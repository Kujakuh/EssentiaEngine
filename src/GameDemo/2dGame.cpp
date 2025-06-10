#include <EssentiaEngine>

#include "headers/demoScene.hpp"

 static void twoDGame()
 {
	std::string title = "Essentia";

	AppConfig config;

	ConfigLoader configLoader(config.setAspectRatio(16, 9)
		.setWindowSize(_WIDTH_)
		.setWindowTitle(title)
		.setMSAASamples(16)
		.enableDepthTest(true)
		.enableBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		.setGLVersion(4, 3)
		.setWireframeMode(false)
		.enableDebugMode(true));


	WindowContext context;
	context.window = configLoader.createWindow();
	context.windowId = 0;

	configLoader.initializeOpenGL(context.window);

	Essentia::init();
	Essentia::initDefaultModels();
	Essentia::render_mode = RENDER_MODE::PBR;
	Essentia::bindlessTexturesMode = true;

	WindowManager* windowManager = WindowManager::GetInstance();
	windowManager->RegisterWindowContext(context);

	demoScene* scene = new demoScene();
	windowManager->ChangeScene(context.windowId, scene);

	InputManager::Initialize(context.window);
	InputManager::SetActiveInstance(context.window);

	while (!glfwWindowShouldClose(context.window))
	{
		InputManager::GetActiveInstance()->Update();
		Time::update();
		auto dt = Time::deltaTime();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		title = "FPS: " + std::to_string(Time::fps());
		glfwSetWindowTitle(context.window, title.c_str());

		windowManager->GetCurrentScene(context.windowId)->Update();

		glfwSwapBuffers(context.window);
		glfwPollEvents();
	}
	glfwDestroyWindow(context.window);

}