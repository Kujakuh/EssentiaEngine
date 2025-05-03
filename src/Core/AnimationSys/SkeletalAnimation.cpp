#include <Core/AnimationSys/SkeletalAnimation.hpp>
#include <future>
#include <vector>
namespace Essentia
{
    SkeletalAnimation::SkeletalAnimation(const std::string& path, Skeleton* skeleton, int animationIndex) : m_Skeleton(skeleton), m_Path(path)
    {
        m_CurrentTime = 0.0f;
        m_Speed = 1.0f;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(m_Path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
            return;
        }

        if (scene &&
            scene->mAnimations != nullptr &&
            animationIndex < scene->mNumAnimations)
        {
            aiAnimation* animation = scene->mAnimations[animationIndex];
            if (animation)
            {
                m_Duration = animation->mDuration;
                m_TicksPerSecond = animation->mTicksPerSecond;

                LoadAnimationBonesKeyframes(animation);
            }
		}
		else
		{
			std::cerr << "ERROR::ASSIMP::No animation found in the file.\n";
			return;
		}
    }

    Bone* SkeletalAnimation::FindBone(const std::string& name)
    {
        auto it = m_BoneMap.find(name);
        if (it != m_BoneMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void SkeletalAnimation::UpdateBoneMap()
    {
        for (auto& bone : m_Bones)
            m_BoneMap[bone.GetBoneName()] = &bone;
    }

    std::unordered_map<std::string, Bone*> SkeletalAnimation::GetBoneMap() { return m_BoneMap; }

    void SkeletalAnimation::LoadAnimationBonesKeyframes(const aiAnimation* animation)
    {
        int size = animation->mNumChannels;

        auto& boneInfoMap = m_Skeleton->GetBoneInfoMap();

        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneInfoMap.size();
            }
            m_Bones.push_back(Bone(channel->mNodeName.data,
                boneInfoMap[channel->mNodeName.data].id, channel));
        }

        m_Skeleton->m_BoneInfoMap = boneInfoMap;
		UpdateBoneMap();
    }

    void SkeletalAnimation::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

        if (Bone* bone = FindBone(nodeName))
        {
            bone->Update(m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        const auto& boneInfoMap = m_Skeleton->m_BoneInfoMap;
        auto it = boneInfoMap.find(nodeName);
        if (it != boneInfoMap.end())
        {
            int index = it->second.id;
            glm::mat4 offset = it->second.offset;
            m_Skeleton->m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; ++i)
        {
            CalculateBoneTransform(&node->children[i], globalTransformation);
        }
    }

    void SkeletalAnimation::Update(float deltaTime)
    { 
		if (m_Speed == 0.0f) return;
		m_CurrentTime += m_TicksPerSecond * deltaTime * m_Speed;
		m_CurrentTime = fmod(m_CurrentTime, m_Duration);
		CalculateBoneTransform(&m_Skeleton->GetRootNode(), glm::mat4(1.0f));
    }
}