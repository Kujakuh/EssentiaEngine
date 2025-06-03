#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include <Core/ECS/ISystem.hpp>
#include <Core/ECS/EntityManager.hpp>
#include <Core/ECS/Entity.hpp>
#include <Framework/Components/Animator.hpp>

namespace Essentia
{
	class AnimationController : public ISystem
	{
		public:
			AnimationController();
			void Update(EntityManager& entityManager) override;	

		private:
			std::vector<WeakptrWrapper<Entity>> animatedEntities;
	};
}
#endif // !ANIM_CONTROLLER_H
