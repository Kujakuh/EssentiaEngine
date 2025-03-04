#ifndef ENTITY_H
#define ENTITY_H

#include <flat_hash_map>

#include <string>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <iostream>

#include <Core/ECS/IComponent.hpp>
#include <Framework/Components/components.hpp>
#include <Core/Exceptions/Exceptions.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>

namespace Essentia
{
    class Entity : std::enable_shared_from_this<Entity>
    {
        private:
            int id;
            std::string name;
            ska::flat_hash_map<std::type_index, std::shared_ptr<IComponent>> components;
            bool isAlive;
            bool isInstance;
            int instanceParentId = -1;

        public:
            Entity(int id, const std::string& name);
            bool active = true;

            int GetID() const;
            const std::string& GetName() const;
            bool IsAlive() const;
            ska::flat_hash_map<std::type_index, std::shared_ptr<IComponent>> GetComponents();
            void Destroy();

            std::function<void()> onUpdate;

        public:
            template <typename T, typename... Args>
            T& AddComponent(Args&&... args)
            {
                if (!isAlive) throw UnreachableEntityException("AddComponent");

                auto component = std::make_shared<T>(std::forward<Args>(args)...);
                components[typeid(T)] = std::move(component);
                return *static_cast<T*>(components[typeid(T)].get());
            }

            template <typename T>
            T* GetComponent()
            {
                if (!isAlive) throw UnreachableEntityException("GetComponent");

                auto it = components.find(typeid(T));
                if (it != components.end()) {
                    return static_cast<T*>(it->second.get());
                }
                return nullptr;
            }

            template <typename T>
            bool HasComponent() const
            {
                if (!isAlive) throw UnreachableEntityException("HasComponent");

                return components.find(typeid(T)) != components.end();
            }

            void defaultUpdate() {}

            friend class EntityManager;
            friend class Scene;
    };
}

#endif // !ENTITY_H