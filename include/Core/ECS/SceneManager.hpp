#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <Core/ECS/Scene.hpp>

#include <memory>

namespace Essentia
{
    class SceneManager 
    {
        private:
            static SceneManager* instance;
            Scene* currentScene;

        public:
            static SceneManager* GetInstance();
            void ChangeScene(Scene* newScene);
            void ChangeScene(Scene* newScene, std::vector<Entity*> entities, std::vector<std::string> arguments);
            Scene* GetCurrentScene();
    };
}

#endif // !SCENE_MANAGER_H