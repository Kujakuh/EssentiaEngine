#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include <Core/ECS/IComponent.hpp>
#include <Core/Components/components.hpp>

namespace Essentia
{
    class Entity 
    {
        private:
            int id;
            std::string name;
            std::unordered_map<std::type_index, std::shared_ptr<IComponent>> components;

        public:
            Entity(int id, const std::string& name);

            int GetID() const;
            const std::string& GetName() const;
            void Destroy();

        public:
            template <typename T, typename... Args>
            T& AddComponent(Args&&... args)
            {
                auto component = std::make_shared<T>(std::forward<Args>(args)...);
                components[typeid(T)] = std::move(component);
                return *static_cast<T*>(components[typeid(T)].get());
            }

            template <typename T>
            T* GetComponent()
            {
                auto it = components.find(typeid(T));
                if (it != components.end()) {
                    return static_cast<T*>(it->second.get());
                }
                return nullptr;
            }

            template <typename T>
            bool HasComponent() const
            {
                return components.find(typeid(T)) != components.end();
            }
    };
}

#endif // !ENTITY_H
