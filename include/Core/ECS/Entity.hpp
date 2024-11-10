#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include <Core/ECS/IComponent.hpp>

namespace EnTT
{
    class Entity {
    private:
        int id;
        std::string name;
        std::unordered_map<std::type_index, std::unique_ptr<IComponent>> components;

    public:
        Entity(int id, const std::string& name);

        int GetID() const;
        void SetID(int id) const;
        const std::string& GetName() const;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template <typename T>
        T* GetComponent();

        template <typename T>
        bool HasComponent() const;

        void Destroy();
    };
}

#endif // !ENTITY_H
