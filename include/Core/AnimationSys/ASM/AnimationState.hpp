#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <string>
#include <vector>
#include <functional>
#include <Core/AnimationSys/Animation.hpp>

namespace Essentia
{
    class AnimationState
    {
        public:
            AnimationState(const std::string& stateName, Animation* anim);
            AnimationState(const AnimationState&) = default;
            AnimationState() = default;

            void AddTransition(const std::string& nextState, std::function<bool()> condition);
            std::string GetNextState();
            Animation* GetAnimation() const;
            std::string GetName() const;

        private:
            std::string name;
            Animation* animation;
            std::vector<std::pair<std::string, std::function<bool()>>> transitions;
    };
}

#endif // !ANIMATIONSTATE_H