#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Essentia
{
    class Keyframe
    {
        public:
            float timeStamp;
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;

            Keyframe(float time, glm::vec3 pos, glm::quat rot, glm::vec3 scl)
                : timeStamp(time), position(pos), rotation(rot), scale(scl) {}

            static Keyframe interpolateKF(const Keyframe& k1, const Keyframe& k2, float factor)
            {
                return Keyframe(
                    glm::mix(k1.timeStamp, k2.timeStamp, factor),
                    glm::mix(k1.position, k2.position, factor),
                    glm::slerp(k1.rotation, k2.rotation, factor),
                    glm::mix(k1.scale, k2.scale, factor)
                );
            }
    };
}

#endif // !KEYFRAME_H