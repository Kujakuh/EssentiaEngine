#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <Core/AnimationSys/BoneSys/Bone.hpp>
#include <Core/AnimationSys/BoneSys/BoneInfo.hpp>
#include <Core/AnimationSys/BoneSys/AssimpNodeData.hpp>

#include <iostream>
#include <assimp/scene.h>

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

            void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
            std::vector<glm::mat4> GetFinalBoneMatrices() const;

            AssimpNodeData& GetRootNode();
            std::map<std::string, BoneInfo>& GetBoneInfoMap();

		friend class SkeletalAnimation;
    };
}

#endif // !SKELETON_H