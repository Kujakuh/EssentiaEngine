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
            std::vector<Bone> m_Bones;
            std::unordered_map<std::string, Bone*> m_BoneMap;
            AssimpNodeData m_RootNode;
            std::map<std::string, BoneInfo> m_BoneInfoMap;
            std::vector<glm::mat4> m_FinalBoneMatrices;

        public:
            Skeleton(const std::vector<Bone>& bones, const AssimpNodeData& rootNode, const std::map<std::string, BoneInfo>& boneInfoMap);
            Skeleton();

            Bone* FindBone(const std::string& name);
            const AssimpNodeData& GetRootNode() const;
            std::map<std::string, BoneInfo>& GetBoneInfoMap();
			std::unordered_map<std::string, Bone*> GetBoneMap();
            std::vector<glm::mat4> GetFinalBoneMatrices();

            void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, float* m_CurrentTime);
    };
}

#endif // !SKELETON_H