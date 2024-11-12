#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <iostream>

#include <Core/ECS/IComponent.hpp>
#include <Core/Components/components.hpp>
#include <Core/Exceptions/Exceptions.hpp>

namespace Essentia
{
    class Entity 
    {
        private:
            int id;
            std::string name;
            std::unordered_map<std::type_index, std::shared_ptr<IComponent>> components;

        public:
            bool isAlive;
            Entity(int id, const std::string& name);

            int GetID() const;
            const std::string& GetName() const;
            std::unordered_map<std::type_index, std::shared_ptr<IComponent>> getComponents();
            void Destroy();

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

            friend class EntityManager;
    };
}

#endif // !ENTITY_H
