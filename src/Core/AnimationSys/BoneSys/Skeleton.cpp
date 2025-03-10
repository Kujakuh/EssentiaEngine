#include <Core/AnimationSys/BoneSys/Skeleton.hpp>

namespace Essentia
{
    Skeleton::Skeleton(const std::vector<Bone>& bones, const AssimpNodeData& rootNode, const std::map<std::string, BoneInfo>& boneInfoMap)
        : m_Bones(bones), m_RootNode(rootNode), m_BoneInfoMap(boneInfoMap)
    {
        for (auto& bone : m_Bones)
            m_BoneMap[bone.GetBoneName()] = &bone;

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

    Bone* Skeleton::FindBone(const std::string& name)
    {
        auto it = m_BoneMap.find(name);
        if (it != m_BoneMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    const AssimpNodeData& Skeleton::GetRootNode() const { return m_RootNode; }
    std::map<std::string, BoneInfo>& Skeleton::GetBoneInfoMap() { return m_BoneInfoMap; }
    std::unordered_map<std::string, Bone*> Skeleton::GetBoneMap() { return m_BoneMap; }
    std::vector<glm::mat4> Skeleton::GetFinalBoneMatrices() { return m_FinalBoneMatrices; }

    void Skeleton::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, float* m_CurrentTime)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

        Bone* bone = FindBone(nodeName);
        if (bone)
        {
            bone->Update(*m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto& boneInfoMap = m_BoneInfoMap;
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneTransform(&node->children[i], globalTransformation, m_CurrentTime);
        }
    }
}