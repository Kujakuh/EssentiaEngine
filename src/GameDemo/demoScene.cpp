#include "headers/demoScene.hpp"

demoScene::demoScene()
{
    camera = new Camera2D("Main Camera", 45.0f, static_cast<float>(_WIDTH_) / _HEIGHT_, 0.1f, 100.0f);
    renderer = Renderer2D(camera);
    animator = AnimationController();
    player = new Player(this);
	skybox = new Skybox(RESOURCES_PATH "Textures/tes.hdr");
}

void demoScene::onInit()
{
    std::cout << "Scene Init" << '\n';
}

void demoScene::onDestroy() {}

void demoScene::onUpdate() 
{
    skybox->render(camera->getProjectionMatrix(), Matrix4(Matrix3(camera->getViewMatrix())));
}

void demoScene::RegisterSystems()
{
    systemDispatcher.RegisterSystem(std::make_shared<Essentia::Renderer2D>(renderer));
    systemDispatcher.RegisterSystem(std::make_shared<Essentia::AnimationController>(animator));
}
