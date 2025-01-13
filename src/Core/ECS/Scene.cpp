#include <Core/ECS/Scene.hpp>

namespace Essentia
{
    WeakptrWrapper<Entity> Scene::CreateEntity(const std::string& name) {return entityManager.CreateEntity(name);}

    EntityManager& Scene::GetEntityManager() {return entityManager;}

    WeakptrWrapper<Entity> Scene::GetEntityByID(int entityId) {return entityManager.GetEntityByID(entityId);}

    WeakptrWrapper<Entity> Scene::GetEntityByName(const std::string& name) { return entityManager.GetEntityByName(name);}

    void Scene::DestroyEntity(WeakptrWrapper<Entity> entity)
    {
        try {
            entityManager.RemoveEntity(entity->GetID());
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    int Scene::Instantiate(WeakptrWrapper<Entity> ent, Transform* transform, float lifetime)
    {
        auto uuid = generateUUID();
        WeakptrWrapper<Entity> instance = CreateEntity(uuid);
        auto targetComponents = ent->GetComponents();

        for (const auto& pair : targetComponents)
            instance->components[pair.first] = pair.second;

        instance->components[std::type_index(typeid(Transform))] = std::make_shared<Transform>(*transform);
        
        if (lifetime != -1)
        {
            std::shared_ptr<Timer> lifeTimeOut= std::make_shared<Timer>(lifetime, [this, instance, lifetime, uuid]()
                {
                    DestroyEntity(instance);
                    std::cout << "Destroyed instance " << uuid <<" after secs: " << lifetime << "\n";
                });

            Time::addTimer(lifeTimeOut);
        }

        return instance->GetID();
    }

    bool Scene::isRunning() const {return running;}

    void Scene::Update()
    {
        auto ents = entityManager.GetEntitiesWith();
        for (WeakptrWrapper<Entity> entity : ents)
            if (entity->IsAlive()) entity->onUpdate();

        if (isRunning())
        {
            systemDispatcher.Dispatch(entityManager);
            onUpdate();
        }
    }

    void Scene::SetActive(bool active)
    {
        if (active && !running) 
        {
            running = true;
            RegisterSystems();
            onInit();
        }
        else if (!active && running) 
        {
            running = false;
            onDestroy();
        }
    }

    void Scene::SetActive(bool active, std::vector<Entity*> entities, std::vector<std::string> arguments)
    {
        if (active && !running)
        {
            running = true;
            RegisterSystems();
            onInit();
            onInit(entities, arguments);
        }
        else if (!active && running)
        {
            running = false;
            onDestroy();
            onDestroy(entities, arguments);
        }
    }

    void Scene::onInit(const std::vector<Entity*>& entities, const std::vector<std::string>& arguments) {}
    void Scene::onDestroy(std::vector<Entity*>& entities, std::vector<std::string>& arguments) {}
    void Scene::onInit() {}
    void Scene::onDestroy() {}
    void Scene::onUpdate() {}
    void Scene::RegisterSystems() {}
}