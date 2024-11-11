#include <Core/ECS/Entity.hpp>

namespace Essentia
{
    Entity::Entity(int id, const std::string& name) : id(id), name(name) {}

    int Entity::GetID() const { return id; }

    const std::string& Entity::GetName() const { return name; }

    void Entity::Destroy() 
    {
        components.clear();
    }
}