#include <Core/AnimationSys/BoneSys/Skeleton.hpp>

namespace Essentia
{
    Skeleton::Skeleton(const AssimpNodeData& rootNode, const std::map<std::string, BoneInfo>& boneInfoMap)
        : m_RootNode(rootNode), m_BoneInfoMap(boneInfoMap)
    {
        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    Skeleton::Skeleton()
    {
        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    const AssimpNodeData& Skeleton::GetRootNode() const { return m_RootNode; }
    std::map<std::string, BoneInfo>& Skeleton::GetBoneInfoMap() { return m_BoneInfoMap; }
    std::vector<glm::mat4> Skeleton::GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

}