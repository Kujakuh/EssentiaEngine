#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <Core/ECS/Scene.hpp>
#include <Framework/Windowing/WindowContext.hpp>

#include <unordered_map>
#include <memory>

namespace Essentia
{
    class WindowManager
    {
        private:
            static WindowManager* instance;
            std::unordered_map<int, WindowContext> contexts;

        public:
            static WindowManager* GetInstance();

            void RegisterWindowContext(const WindowContext& context);
            void RemoveWindowContext(int windowId);

            void AddScene(int windowId, Scene* scene);
            void ChangeScene(int windowId, Scene* newScene);
            void ChangeScene(int windowId, Scene* newScene, std::vector<Entity*> entities, std::vector<std::string> arguments);

            Scene* GetCurrentScene(int windowId);
            std::vector<Scene*> GetUpdatedScenes(int windowId);

            const std::unordered_map<int, WindowContext>& GetAllContexts() const;
    };
}

#endif // !WINDOW_MANAGER_H
