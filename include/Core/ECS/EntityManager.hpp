#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>
#include <Core/ECS/Entity.hpp>

namespace EnTT
{
    class EntityManager {
    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::queue<int> availableIDs;

    public:
        Entity& CreateEntity(const std::string& name);
        Entity* GetEntityByID(int entityId);
        void RemoveEntity(int entityId);

        template <typename... Components>
        std::vector<Entity*> GetEntitiesWith();
    };
}

#endif // ENTITY_MANAGER_H
