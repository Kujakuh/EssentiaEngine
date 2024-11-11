#include <Core/ECS/SceneManager.hpp>

namespace EnTT
{
    SceneManager* SceneManager::instance = nullptr;

    SceneManager* SceneManager::GetInstance()
    {
        if (!instance) instance = new SceneManager();
        return instance;
    }

    void SceneManager::ChangeScene(Scene* newScene)
    {
        if (currentScene) currentScene->SetActive(false);
        currentScene = newScene;
        currentScene->SetActive(true);
    }

    void SceneManager::ChangeScene(Scene* newScene, std::vector<Entity*> entities, std::vector<std::string> arguments)
    {
        if (currentScene) currentScene->SetActive(false, entities, arguments);
        currentScene = newScene;
        currentScene->SetActive(true, entities, arguments);
    }

    Scene* SceneManager::GetCurrentScene() {return currentScene;}
}