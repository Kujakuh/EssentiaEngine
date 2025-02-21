#ifndef BONE_H
#define BONE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/anim.h>
#include <Core/AnimationSystem/Keyframe.hpp>
#include <Core/Graphics/assimp_glm_helpers.hpp>

namespace Essentia
{
    class Bone
    {
        private:
            std::vector<Keyframe> m_Keyframes;
            glm::mat4 m_LocalTransform;
            std::string m_Name;
            int m_ID;

        public:
            Bone(const std::string& name, int ID, const aiNodeAnim* channel);
            void Update(float animationTime);
            glm::mat4 GetLocalTransform() const;
            std::string GetBoneName() const;
            int GetBoneID() const;

        private:
            int GetKeyframeIndex(float animationTime) const;
            float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const;
            glm::mat4 Interpolate(float animationTime) const;
    };
}

#endif // !BONE_H