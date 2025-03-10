#include <Core/AnimationSys/ASM/AnimationState.hpp>

namespace Essentia
{
    AnimationState::AnimationState(const std::string& stateName, IAnimation* anim)
        : name(stateName), animation(anim) {}

    void AnimationState::AddTransition(const std::string& nextState, std::function<bool()> condition)
    {
        transitions.emplace_back(nextState, condition);
    }

    std::string AnimationState::GetNextState()
    {
        for (auto& [state, condition] : transitions)
        {
            if (condition()) return state;
        }
        return name;
    }

    IAnimation* AnimationState::GetAnimation() const
    {
        return animation;
    }

    std::string AnimationState::GetName() const { return name; }
}