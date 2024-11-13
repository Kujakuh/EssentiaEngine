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


    bool Scene::isRunning() const {return running;}

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
            inputOnInit(entities, arguments);
        }
        else if (!active && running)
        {
            running = false;
            onDestroy();
            outputOnDestroy(entities, arguments);
        }
    }

    void Scene::inputOnInit(const std::vector<Entity*>& entities, const std::vector<std::string>& arguments) {}
    void Scene::outputOnDestroy(std::vector<Entity*>& entities, std::vector<std::string>& arguments) {}
    void Scene::onInit() {}
    void Scene::onDestroy() {}
    void Scene::RegisterSystems() {}

}