#ifndef ANIMATION_H
#define ANIMATION_H

#include <unordered_map>
#include <string>

#include <Core/AnimationSys/Keyframe.hpp>

namespace Essentia
{
    class IAnimation
    {
        protected:
            float m_Duration;
            float m_TicksPerSecond;
			float m_CurrentTime = 0.0f;
            std::unordered_map<std::string, std::vector<Keyframe>> m_Keyframes;

        public:
            IAnimation(float duration, float ticksPerSecond)
                : m_Duration(duration), m_TicksPerSecond(ticksPerSecond) {}

            virtual ~IAnimation() = default;
            virtual void Update(float animationTime) = 0;

			void Reset() { m_CurrentTime = 0.0f; }

            float GetDuration() const { return m_Duration; }
            float GetTicksPerSecond() const { return m_TicksPerSecond; }
    };
}

#endif // !ANIMATION_H