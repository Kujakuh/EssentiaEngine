#include <EssentiaEngine>

#include "../Testing/SceneTemplate.cpp"

constexpr int _WIDTH = 900;
constexpr int _HEIGHT = (int)(0.5625 * _WIDTH);

 static void twoDGame()
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
}