#include <Core/ECS/EntityManager.hpp>

namespace Essentia
{
    std::weak_ptr<Entity> EntityManager::CreateEntity(const std::string& name)
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
        return entities[entityId];
    }

    std::weak_ptr<Entity> EntityManager::GetEntityByID(int entityId) 
    {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            return entities[entityId];
        }
        return std::weak_ptr<Entity>();
    }

    std::weak_ptr<Entity> EntityManager::GetEntityByName(const std::string& name)
    {
        auto it = std::find_if(entities.begin(), entities.end(),
            [&name](const std::weak_ptr<Entity>& entity)
            {
                return entity.lock()->GetName() == name;
            });

        return it != entities.end() ? std::weak_ptr<Entity>(entities[it - entities.begin()]) : std::weak_ptr<Entity>();
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