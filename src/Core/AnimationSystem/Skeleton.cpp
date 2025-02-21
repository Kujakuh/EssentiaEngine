#include <Core/AnimationSystem/Skeleton.hpp>

namespace Essentia
{
    Skeleton::Skeleton(const std::vector<Bone>& bones, const AssimpNodeData& rootNode, const std::map<std::string, BoneInfo>& boneInfoMap)
        : m_Bones(bones), m_RootNode(rootNode), m_BoneInfoMap(boneInfoMap)
    {
        for (auto& bone : m_Bones)
        {
            m_BoneMap[bone.GetBoneName()] = &bone;
        }
    }

    Bone* Skeleton::FindBone(const std::string& name)
    {
        auto it = m_BoneMap.find(name);
        if (it != m_BoneMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    const AssimpNodeData& Skeleton::GetRootNode() const
    {
        return m_RootNode;
    }

    const std::map<std::string, BoneInfo>& Skeleton::GetBoneInfoMap() const
    {
        return m_BoneInfoMap;
    }
}