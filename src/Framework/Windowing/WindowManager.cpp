#include <Framework/Windowing/WindowManager.hpp>

namespace Essentia
{
    WindowManager* WindowManager::instance = nullptr;

    WindowManager* WindowManager::GetInstance()
    {
        if (!instance) instance = new WindowManager();
        return instance;
    }

    void WindowManager::RegisterWindowContext(const WindowContext& context)
    {
        contexts[context.windowId] = context;
    }

    void WindowManager::RemoveWindowContext(int windowId)
    {
        contexts.erase(windowId);
    }

    void WindowManager::AddScene(int windowId, Scene* scene)
    {
        auto& context = contexts[windowId];
        auto it = std::find(context.updatedScenes.begin(), context.updatedScenes.end(), scene);
        if (it == context.updatedScenes.end())
        {
            context.updatedScenes.push_back(scene);
        }
    }

    void WindowManager::ChangeScene(int windowId, Scene* newScene)
    {
        auto& context = contexts[windowId];

        if (context.activeScene && context.activeScene != newScene)
        {
            context.activeScene->SetActive(false);
        }

        AddScene(windowId, newScene);

        context.activeScene = newScene;
        newScene->SetActive(true);
    }

    void WindowManager::ChangeScene(int windowId, Scene* newScene, std::vector<Entity*> entities, std::vector<std::string> arguments)
    {
        auto& context = contexts[windowId];

        if (context.activeScene && context.activeScene != newScene)
        {
            context.activeScene->SetActive(false, entities, arguments);
        }

        AddScene(windowId, newScene);

        context.activeScene = newScene;
        newScene->SetActive(true, entities, arguments);
    }

    Scene* WindowManager::GetCurrentScene(int windowId)
    {
        auto it = contexts.find(windowId);
        if (it != contexts.end())
            return it->second.activeScene;

        return nullptr;
    }

    std::vector<Scene*> WindowManager::GetUpdatedScenes(int windowId)
    {
        return contexts[windowId].updatedScenes;
    }

    const std::unordered_map<int, WindowContext>& WindowManager::GetAllContexts() const
    {
        return contexts;
    }
}
