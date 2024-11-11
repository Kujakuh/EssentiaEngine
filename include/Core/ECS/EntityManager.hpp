#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>
#include <Core/ECS/Entity.hpp>

namespace Essentia
{
    class EntityManager 
    {
        private:
            std::vector<std::unique_ptr<Entity>> entities;
            std::queue<int> availableIDs;

        public:
            Entity& CreateEntity(const std::string& name);
            Entity* GetEntityByID(int entityId);
            Entity* GetEntityByName(const std::string& name);
            void RemoveEntity(int entityId);

        public:
            template <typename... Components>
            std::vector<Entity*> GetEntitiesWith()
            {
                std::vector<Entity*> result;

                for (auto& entity : entities)
                {
                    if (entity)
                    {
                        bool hasAllComponents = true;
                        (void)std::initializer_list<int>{(hasAllComponents &= entity->HasComponent<Components>(), 0)...};

                        if (hasAllComponents) result.push_back(entity.get());
                    }
                }
                return result;
            }
    };
}

#endif // !ENTITY_MANAGER_H
