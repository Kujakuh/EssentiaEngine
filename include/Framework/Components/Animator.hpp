#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <Core/ECS/IComponent.hpp>
#include <Core/AnimationSys/ASM/AnimationStateMachine.hpp>
#include <memory>
#include <string>

namespace Essentia
{
    class Animator : public IComponent
    {
        public:
            Animator() = default;

            void AddState(const std::string& name, IAnimation* animation);
            void AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition);
            void Update(float deltaTime);

            IAnimation* GetCurrentAnimation() const;

        private:
            AnimationStateMachine stateMachine;
    };
}

#endif // !ANIMATOR_H
