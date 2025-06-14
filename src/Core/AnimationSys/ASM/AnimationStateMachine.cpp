#include <Core/AnimationSys/ASM/AnimationStateMachine.hpp>

namespace Essentia
{
    AnimationStateMachine::AnimationStateMachine() : currentState(nullptr) {}

    void AnimationStateMachine::AddState(const std::string& name, IAnimation* animation)
    {
        states[name] = AnimationState(name, animation);
        if (!currentState) currentState = &states[name];
    }

    void AnimationStateMachine::AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition)
    {
        states[from].AddTransition(to, condition);
    }

    void AnimationStateMachine::Update(float deltaTime)
    {
        if (!currentState) return;
        auto state = currentState->GetNextState();

        if (currentState->GetName() != state)
        {
            currentState = &states[state];
            currentState->GetAnimation()->Reset();
        }
       
        currentState->GetAnimation()->Update(deltaTime);
    }

    IAnimation* AnimationStateMachine::GetCurrentAnimation() const
    {
        return currentState ? currentState->GetAnimation() : nullptr;
    }

}