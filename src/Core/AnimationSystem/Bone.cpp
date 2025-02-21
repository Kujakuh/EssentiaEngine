#include <Core/AnimationSystem/Bone.hpp>

namespace Essentia
{
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        : m_Name(name), m_ID(ID), m_LocalTransform(1.0f)
    {
        int numKeyframes = channel->mNumPositionKeys;
        for (int i = 0; i < numKeyframes; ++i)
        {
            float timeStamp = channel->mPositionKeys[i].mTime;
            glm::vec3 position = AssimpGLMHelpers::GetGLMVec(channel->mPositionKeys[i].mValue);
            glm::quat rotation = AssimpGLMHelpers::GetGLMQuat(channel->mRotationKeys[i].mValue);
            glm::vec3 scale = AssimpGLMHelpers::GetGLMVec(channel->mScalingKeys[i].mValue);
            m_Keyframes.emplace_back(timeStamp, position, rotation, scale);
        }
    }

    void Bone::Update(float animationTime)
    {
        m_LocalTransform = Interpolate(animationTime);
    }

    glm::mat4 Bone::GetLocalTransform() const { return m_LocalTransform; }
    std::string Bone::GetBoneName() const { return m_Name; }
    int Bone::GetBoneID() const { return m_ID; }

    int Bone::GetKeyframeIndex(float animationTime) const
    {
        for (size_t i = 0; i < m_Keyframes.size() - 1; ++i)
        {
            if (animationTime < m_Keyframes[i + 1].timeStamp)
                return i;
        }
        return static_cast<int>(m_Keyframes.size() - 1);
    }

    float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const
    {
        return (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
    }

    glm::mat4 Bone::Interpolate(float animationTime) const
    {
        if (m_Keyframes.size() == 1)
        {
            return glm::translate(glm::mat4(1.0f), m_Keyframes[0].position) *
                glm::mat4_cast(m_Keyframes[0].rotation) *
                glm::scale(glm::mat4(1.0f), m_Keyframes[0].scale);
        }

        int index = GetKeyframeIndex(animationTime);
        int nextIndex = index + 1;
        float factor = GetScaleFactor(m_Keyframes[index].timeStamp, m_Keyframes[nextIndex].timeStamp, animationTime);

        Keyframe interpolatedKF = Keyframe::interpolateKF(m_Keyframes[index], m_Keyframes[nextIndex], factor);

        return glm::translate(glm::mat4(1.0f), interpolatedKF.position) *
            glm::mat4_cast(interpolatedKF.rotation) *
            glm::scale(glm::mat4(1.0f), interpolatedKF.scale);
    }
}