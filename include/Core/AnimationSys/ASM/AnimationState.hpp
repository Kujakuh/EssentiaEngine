#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <string>
#include <vector>
#include <functional>
#include <Core/AnimationSys/IAnimation.hpp>

namespace Essentia
{
    class AnimationState
    {
        public:
            AnimationState(const std::string& stateName, IAnimation* anim);
            AnimationState(const AnimationState&) = default;
            AnimationState() = default;

            void AddTransition(const std::string& nextState, std::function<bool()> condition);
            std::string GetNextState();
            IAnimation* GetAnimation() const;
            std::string GetName() const;

        private:
            std::string name;
            IAnimation* animation;
            std::vector<std::pair<std::string, std::function<bool()>>> transitions;
    };
}

#endif // !ANIMATIONSTATE_H