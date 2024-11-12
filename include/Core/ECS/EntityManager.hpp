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
            std::vector<std::shared_ptr<Entity>> entities;
            std::queue<int> availableIDs;

        public:
            std::weak_ptr<Entity> CreateEntity(const std::string& name);
            std::weak_ptr<Entity> GetEntityByID(int entityId);
            std::weak_ptr<Entity> GetEntityByName(const std::string& name);
            void RemoveEntity(int entityId);

        public:
            template <typename... Components>
            std::vector<std::weak_ptr<Entity>> GetEntitiesWith()
            {
                std::vector<std::weak_ptr<Entity>> result;

                for (auto& entity : entities)
                {
                    if (entity)
                    {
                        bool hasAllComponents = true;
                        (void)std::initializer_list<int>{(hasAllComponents &= entity->HasComponent<Components>(), 0)...};

                        if (hasAllComponents)
                            result.push_back(std::weak_ptr<Entity>(entity));
                    }
                }
                return result;
            }

    };
}

#endif // !ENTITY_MANAGER_H
