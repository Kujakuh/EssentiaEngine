#include <Core/ECS/EntityManager.hpp>

namespace EnTT
{
    Entity& EntityManager::CreateEntity(const std::string& name) {
        int entityId;

        // Reutilizar un ID disponible o asignar uno nuevo
        if (!availableIDs.empty()) {
            entityId = availableIDs.front();
            availableIDs.pop();
        }
        else {
            entityId = static_cast<int>(entities.size());
            entities.emplace_back(nullptr); // Reservar espacio en el vector
        }

        // Crear y almacenar la entidad
        auto entity = std::make_unique<Entity>(entityId, name);
        entities[entityId] = std::move(entity);
        return *entities[entityId];
    }

    Entity* EntityManager::GetEntityByID(int entityId) {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) {
            return entities[entityId].get();
        }
        return nullptr;  // La entidad no existe
    }

    void EntityManager::RemoveEntity(int entityId) {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) {
            entities[entityId].reset();  // Eliminar la entidad
            availableIDs.push(entityId); // Guardar el ID para reutilizar
        }
    }

    template <typename... Components>
    std::vector<Entity*> EntityManager::GetEntitiesWith() {
        std::vector<Entity*> result;

        for (auto& entity : entities) {
            if (entity) {
                bool hasAllComponents = true;
                (void)std::initializer_list<int>{(hasAllComponents &= entity->HasComponent<Components>(), 0)...};

                if (hasAllComponents) {
                    result.push_back(entity.get());
                }
            }
        }
        return result;
    }
}
