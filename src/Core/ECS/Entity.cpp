#include <Core/ECS/Entity.hpp>

namespace Essentia
{
    Entity::Entity(int id, const std::string& name) : id(id), name(name), isAlive(true) 
    {
        onUpdate = [this]() {this->defaultUpdate();};
    }

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

    bool Entity::IsAlive() const { return isAlive; }

    ska::flat_hash_map<std::type_index, std::weak_ptr<IComponent>> Entity::GetComponents()
    {
        ska::flat_hash_map<std::type_index, std::weak_ptr<IComponent>> weakComponents;
        for (const auto& component : components)
        {
            weakComponents[component.first] = component.second;
        }
        return weakComponents;
    }

    void Entity::Destroy()
    {
        if (!isAlive) throw UnreachableEntityException("Destroy");

        isAlive = false;
        for (auto& component : components) component.second.reset();
        components.clear(); 
    }

    void Entity::SetParent(WeakptrWrapper<Entity> newParent)
    {
        if (newParent.get() == this)
            throw std::invalid_argument("An entity cannot be its own parent.");

        if (newParent.lock() && newParent.lock()->IsAncestorOf(WeakptrWrapper<Entity>(shared_from_this())))
            throw std::invalid_argument("Cannot set a descendant as parent.");

        // Eliminar esta entidad de los hijos del padre actual
        if (auto currentParent = parent.lock()) {
            currentParent->RemoveChild(WeakptrWrapper<Entity>(shared_from_this()));
        }

        parent = newParent;

        if (newParent.lock()) newParent.lock()->AddChild(WeakptrWrapper<Entity>(shared_from_this()));
    }

    WeakptrWrapper<Entity> Entity::GetParent() const { return parent; }

    void Entity::AddChild(WeakptrWrapper<Entity> child)
    {
        if (child.get() == this)
            throw std::invalid_argument("An entity cannot be its own child.");

        if (std::find_if(children.begin(), children.end(),
            [&](const WeakptrWrapper<Entity>& c) { return c.lock() == child.lock(); }) == children.end())
        {
            children.push_back(child);
        }
    }

    void Entity::RemoveChild(WeakptrWrapper<Entity> child)
    {
        children.erase(std::remove_if(children.begin(), children.end(),
            [&](const WeakptrWrapper<Entity>& c) { return c.lock() == child.lock(); }),
            children.end());
    }

    std::vector<WeakptrWrapper<Entity>> Entity::GetChildren() const { return children; }

    bool Entity::IsAncestorOf(WeakptrWrapper<Entity> entity) const
    {
        auto current = entity.lock();
        while (current) {
            if (current.get() == this) {
                return true;
            }
            current = current->GetParent().lock();
        }
        return false;
    }

}