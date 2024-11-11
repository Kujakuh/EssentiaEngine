#include <Core/ECS/Scene.hpp>

namespace EnTT
{
    Entity& Scene::CreateEntity(const std::string& name) {return entityManager.CreateEntity(name);}

    EntityManager& Scene::GetEntityManager() {return entityManager;}

    void Scene::RemoveEntity(int entityId) {entityManager.RemoveEntity(entityId);}

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