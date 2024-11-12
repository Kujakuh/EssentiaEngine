#include <Core/ECS/EntityManager.hpp>

namespace Essentia
{
    Entity& EntityManager::CreateEntity(const std::string& name) 
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
        auto entity = std::make_unique<Entity>(entityId, name);
        entities[entityId] = std::move(entity);
        entities[entityId].get()->AddComponent<Transform>();
        return *entities[entityId];
    }

    Entity* EntityManager::GetEntityByID(int entityId) 
    {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            return entities[entityId].get();
        }
        return nullptr;
    }

    Entity* EntityManager::GetEntityByName(const std::string& name)
    {
        auto it = std::find_if(entities.begin(), entities.end(),
            [&name](const std::unique_ptr<Entity>& entity) 
            {
                return entity->GetName() == name; 
            });

        return it != entities.end() ? it->get() : nullptr;
    }

    void EntityManager::RemoveEntity(Entity& entity)
    {
        int entityId = entity.GetID();

        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            entities[entityId]->Destroy();
            entities[entityId].reset();
            availableIDs.push(entityId);
        }
    }
}