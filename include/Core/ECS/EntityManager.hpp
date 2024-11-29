#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>

#include <Core/ECS/Entity.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>

namespace Essentia
{
    class EntityManager 
    {
        private:
            std::vector<std::shared_ptr<Entity>> entities;
            std::queue<int> availableIDs;

        public:
            WeakptrWrapper<Entity> CreateEntity(const std::string& name);
            WeakptrWrapper<Entity> GetEntityByID(int entityId);
            WeakptrWrapper<Entity> GetEntityByName(const std::string& name);
            void RemoveEntity(int entityId);

        public:
            template <typename... Components>
            std::vector<WeakptrWrapper<Entity>> GetEntitiesWith()
            {
                std::vector<WeakptrWrapper<Entity>> result;

                for (auto& entity : entities)
                {
                    if (entity)
                    {
                        bool hasAllComponents = true;
                        (void)std::initializer_list<int>{(hasAllComponents &= entity->HasComponent<Components>(), 0)...};

                        if (hasAllComponents)
                            result.emplace_back(WeakptrWrapper<Entity>(entity));
                    }
                }
                return result;
            }


    };
}

#endif // !ENTITY_MANAGER_H