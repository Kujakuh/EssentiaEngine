#include <Core/ECS/SystemDispatcher.hpp>

namespace EnTT
{
    void SystemDispatcher::RegisterSystem(std::shared_ptr<ISystem> system) {
        systems.push_back(system);
    }

    void SystemDispatcher::Dispatch(EntityManager& entityManager) {
        for (auto& system : systems) {
            system->Update(entityManager);
        }
    }
}