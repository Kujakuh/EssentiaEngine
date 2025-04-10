#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <Core/AnimationSys/BoneSys/Bone.hpp>
#include <Core/AnimationSys/BoneSys/BoneInfo.hpp>
#include <Core/AnimationSys/BoneSys/AssimpNodeData.hpp>

namespace Essentia
{
    class Skeleton
    {
        private:
            AssimpNodeData m_RootNode;
            std::map<std::string, BoneInfo> m_BoneInfoMap;
            std::vector<glm::mat4> m_FinalBoneMatrices;

        public:
            Skeleton(const AssimpNodeData& rootNode, const std::map<std::string, BoneInfo>& boneInfoMap);
            Skeleton();

            const AssimpNodeData& GetRootNode() const;
            std::map<std::string, BoneInfo>& GetBoneInfoMap();
            std::vector<glm::mat4> GetFinalBoneMatrices() const;

		friend class SkeletalAnimation;
    };
}

#endif // !SKELETON_H