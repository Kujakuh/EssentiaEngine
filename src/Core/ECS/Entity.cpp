#include <Core/ECS/Entity.hpp>

namespace Essentia
{
    Entity::Entity(int id, const std::string& name) : id(id), name(name), isAlive(true) {}

    int Entity::GetID() const 
    {
        if (!isAlive) throw UnreachableEntityException("GetID");
        return id;
    }

    const std::string& Entity::GetName() const 
    {
        if (!isAlive) throw UnreachableEntityException("GetName");
        return name; 
    }

    void Entity::Destroy()
    {
        if (!isAlive) throw UnreachableEntityException("Destroy");
        isAlive = false;
        for (auto& component : components) component.second.reset();
        components.clear(); 
    }
}