#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <Core/ECS/EntityManager.hpp>

namespace EnTT
{
    class ISystem 
    {
        public:
            virtual void Update(EntityManager& entityManager) = 0;
            virtual ~ISystem() = default;
    };
}

#endif // !ISYSTEM_H
