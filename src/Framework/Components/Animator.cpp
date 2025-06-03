#include <Framework/Components/Animator.hpp>

namespace Essentia
{
    void Animator::AddState(const std::string& name, IAnimation* animation)
    {
        stateMachine.AddState(name, animation);
    }

    void Animator::AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition)
    {
        stateMachine.AddTransition(from, to, condition);
    }

    void Animator::Update(float deltaTime)
    {
        stateMachine.Update(deltaTime);
    }

    IAnimation* Animator::GetCurrentAnimation() const
    {
        return stateMachine.GetCurrentAnimation();
    }
}
