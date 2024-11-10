#include <Core/ECS/Entity.hpp>

namespace EnTT
{
    Entity::Entity(int id, const std::string& name) : id(id), name(name) {}

    int Entity::GetID() const { return id; }
    void Entity::SetID(int id) const { id = id; }

    const std::string& Entity::GetName() const { return name; }

    template <typename T, typename... Args>
    T& Entity::AddComponent(Args&&... args) {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        components[typeid(T)] = component;
        return *component;
    }

    template <typename T>
    T* Entity::GetComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template <typename T>
    bool Entity::HasComponent() const {
        return components.find(typeid(T)) != components.end();
    }

    void Entity::Destroy() {
        components.clear();
    }
}
