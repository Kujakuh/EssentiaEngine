#ifndef SYSTEM_DISPATCHER_H
#define SYSTEM_DISPATCHER_H

#include <vector>
#include <memory>
#include <Core/ECS/ISystem.hpp>
#include <Core/ECS/EntityManager.hpp>

namespace Essentia
{
    class SystemDispatcher 
    {
        private:
            std::vector<std::shared_ptr<ISystem>> systems;

        public:
            void RegisterSystem(std::shared_ptr<ISystem> system);
            void Dispatch(EntityManager& entityManager);
    };
}

#endif // !SYSTEM_DISPATCHER_H
