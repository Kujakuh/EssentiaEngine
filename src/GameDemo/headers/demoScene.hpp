#ifndef DEMO_SCENE_HPP
#define DEMO_SCENE_HPP

#include <EssentiaEngine>
#include <iostream>
#include "definitions.hpp"
#include "player.hpp"

class demoScene : public Scene
{
    public:
        Camera2D *camera;
        Renderer2D renderer;
        AnimationController animator;
		Player* player;
        Skybox* skybox;

        demoScene();

        void onInit() override;
        void onDestroy() override;
        void onUpdate() override;
        void RegisterSystems() override;
};

#endif // DEMO_SCENE_HPP