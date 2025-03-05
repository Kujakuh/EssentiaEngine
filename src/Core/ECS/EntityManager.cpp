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

	std::map<int, std::vector<std::string>> EntityManager::GetInstances() { return instances; }

    void EntityManager::RemoveEntity(int entityId)
    {
        if (entityId >= 0 && entityId < entities.size() && entities[entityId]) 
        {
            if (entities[entityId]->instanceParentId != -1)
            {
				instances[entities[entityId]->instanceParentId].erase(
                    std::remove(
                        instances[entities[entityId]->instanceParentId].begin(),
                        instances[entities[entityId]->instanceParentId].end(),
                        entities[entityId]->GetName()),
                    instances[entities[entityId]->instanceParentId].end()
                );
            }
            entities[entityId]->Destroy();
            entities[entityId].reset();
            availableIDs.push(entityId);
        }
    }

    int EntityManager::Instantiate(WeakptrWrapper<Entity> ent, Transform* transform, float lifetime)
    {
        auto uuid = generateUUID();
        WeakptrWrapper<Entity> instance = CreateEntity(uuid);
		instance->instanceParentId = ent->GetID();
        auto targetComponents = ent->GetComponents();

        for (const auto& pair : targetComponents)
            instance->components[pair.first] = pair.second;

        instance->components[std::type_index(typeid(Transform))] = std::make_shared<Transform>(*transform);

		instances[ent->GetID()].push_back(uuid);

        if (lifetime != -1)
        {
            std::shared_ptr<Timer> lifeTimeOut = std::make_shared<Timer>(lifetime, [this, instance, lifetime, uuid]()
                {
                    try {
                        RemoveEntity(instance.weakPtr_.lock()->GetID());
                        std::cout << "Destroyed instance " << uuid << " after secs: " << lifetime << "\n";
                    }
                    catch (const std::runtime_error& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                });

            Time::addTimer(lifeTimeOut);
        }

        return instance->GetID();
    }

    int EntityManager::Instantiate(WeakptrWrapper<Entity> ent, glm::vec3 position, float lifetime)
    {
        Transform* temp = new Transform();
        temp->setPosition(position);
        return Instantiate(ent, temp, lifetime);
    }
}