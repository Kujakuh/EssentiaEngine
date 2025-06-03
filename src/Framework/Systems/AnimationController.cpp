#include <Framework/Systems/AnimationController.hpp>

namespace Essentia 
{
	AnimationController::AnimationController()
	{
		std::vector<WeakptrWrapper<Entity>> animatedEntities;
	}

	void AnimationController::Update(EntityManager& entityManager)
	{
		animatedEntities = entityManager.GetEntitiesWith<Animator>();

		for (auto& entity : animatedEntities)
			entity->GetComponent<Animator>()->Update(Time::deltaTime());
	};

}