#include <Core/AnimationSys/ASM/AnimationStateMachine.hpp>

namespace Essentia
{
    AnimationStateMachine::AnimationStateMachine() : currentState(nullptr) {}

    void AnimationStateMachine::AddState(const std::string& name, Animation* animation)
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
        if (currentState->GetName() != currentState->GetNextState()) return;

        currentState = &states[nextState];
        //currentState->GetAnimation()->Reset();

        currentState->GetAnimation()->Update(deltaTime);
    }

    Animation* AnimationStateMachine::GetCurrentAnimation() const
    {
        return currentState ? currentState->GetAnimation() : nullptr;
    }

}