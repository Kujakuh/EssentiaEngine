#include <Core/ECS/EntityManager.hpp>

namespace Essentia
{
    WeakptrWrapper<Entity> EntityManager::CreateEntity(const std::string& name)
    {
        int entityId;

        if (!availableIDs.empty()) 
        {
            entityId = availableIDs.front();
            availableIDs.pop();
        }
        else 
        {
            entityId = static_cast<int>(entities.size());
            entities.emplace_back(nullptr); // Reservar espacio en el vector
        }

        // Crear y almacenar la entidad
        auto entity = std::make_shared<Entity>(entityId, name);
        entities[entityId] = std::move(entity);
        entities[entityId].get()->AddComponent<Transform>();
        return WeakptrWrapper<Entity>(entities[entityId]);
    }

    WeakptrWrapper<Entity> EntityManager::GetEntityByID(int entityId)
    {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            return WeakptrWrapper<Entity>(entities[entityId]);
        }
        return WeakptrWrapper<Entity>(std::weak_ptr<Entity>());
    }

    WeakptrWrapper<Entity> EntityManager::GetEntityByName(const std::string& name)
    {
        auto it = std::find_if(entities.begin(), entities.end(),
            [&name](const std::weak_ptr<Entity>& entity)
            {
                auto sharedEntity = entity.lock();
                return sharedEntity && sharedEntity->GetName() == name;
            });

        if (it != entities.end())
        {
            return WeakptrWrapper<Entity>(*it);
        }

        return WeakptrWrapper<Entity>(std::weak_ptr<Entity>());
    }


    void EntityManager::RemoveEntity(int entityId)
    {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            entities[entityId]->Destroy();
            entities[entityId].reset();
            availableIDs.push(entityId);
        }
    }
}