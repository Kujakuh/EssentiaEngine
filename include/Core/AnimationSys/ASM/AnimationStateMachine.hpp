#ifndef ASM_H
#define ASM_H

#include <unordered_map>
#include <Core/AnimationSys/ASM/AnimationState.hpp>

namespace Essentia
{
    class AnimationStateMachine
    {
        public:
            AnimationStateMachine();

            void AddState(const std::string& name, Animation* animation);
            void AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition);
            void Update(float deltaTime);
            Animation* GetCurrentAnimation() const;

        private:
            std::unordered_map<std::string, AnimationState> states;
            AnimationState* currentState;
        };
}

#endif // !ASM_H