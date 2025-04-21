#ifndef SKELETAL_ANIM_H
#define SKELETAL_ANIM_H

#include <Core/AnimationSys/IAnimation.hpp>
#include <Core/AnimationSys/BoneSys/Skeleton.hpp>
#include <Core/AnimationSys/BoneSys/AssimpNodeData.hpp>
#include <Core/Graphics/assimp_glm_helpers.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

namespace Essentia
{

	class SkeletalAnimation : public IAnimation
	{
		public:
			SkeletalAnimation(const std::string& path, Skeleton* skeleton, int animationIndex = 0);
			SkeletalAnimation(const SkeletalAnimation&) = default;
			SkeletalAnimation() = default;

			void Update(float deltaTime) override;

			//void SetCurrentTime(float time);
			//float GetCurrentTime() const;

			//void SetSpeed(float speed);
			//float GetSpeed() const;

		private:

			void LoadAnimationBonesKeyframes(const aiAnimation* animation);
			
			void UpdateBoneMap();
			void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
			Bone* FindBone(const std::string& name);

			std::unordered_map<std::string, Bone*> GetBoneMap();

			std::vector<Bone> m_Bones;
			std::unordered_map<std::string, Bone*> m_BoneMap;

			std::string m_Path;
			Skeleton* m_Skeleton;

			float m_TicksPerSecond;
	};
}

#endif // !SKELETAL_ANIM_H