#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <Core/ECS/EntityManager.hpp>
#include <Core/ECS/SystemDispatcher.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>

namespace Essentia
{
    class Scene 
    {
        protected:
            EntityManager entityManager;
            bool running = false;
            SystemDispatcher* systemDispatcher;

        public:
            virtual ~Scene() = default;

            WeakptrWrapper<Entity> CreateEntity(const std::string& name);
            EntityManager& GetEntityManager();
            WeakptrWrapper<Entity> GetEntityByID(int entityId);
            WeakptrWrapper<Entity> GetEntityByName(const std::string& name);
            void DestroyEntity(WeakptrWrapper<Entity> entity);

            void SetActive(bool active);
            void SetActive(bool active, std::vector<Entity*> entities, std::vector<std::string> arguments);
            bool isRunning() const;

        protected:
            virtual void RegisterSystems();

            virtual void onInit();
            virtual void onDestroy();

            virtual void inputOnInit(const std::vector<Entity*>& entities = {}, const std::vector<std::string>& arguments = {});
            virtual void outputOnDestroy(std::vector<Entity*>& entities, std::vector<std::string>& arguments);

        public:
            template <typename... Components>
            decltype(auto) GetEntitiesWith() {return entityManager.GetEntitiesWith<Components...>();}
    };
}

#endif // !SCENE_H