#ifndef ANIMATION_H
#define ANIMATION_H

#include <unordered_map>
#include <string>

#include <Core/AnimationSys/Keyframe.hpp>

namespace Essentia
{
    class Animation
    {
        protected:
            float m_Duration;
            float m_TicksPerSecond;
            std::unordered_map<std::string, std::vector<Keyframe>> m_Keyframes;

        public:
            Animation(float duration, float ticksPerSecond)
                : m_Duration(duration), m_TicksPerSecond(ticksPerSecond) {}

            virtual ~Animation() = default;
            virtual void Update(float animationTime) = 0;

            float GetDuration() const { return m_Duration; }
            float GetTicksPerSecond() const { return m_TicksPerSecond; }
    };
}

#endif // !ANIMATION_H