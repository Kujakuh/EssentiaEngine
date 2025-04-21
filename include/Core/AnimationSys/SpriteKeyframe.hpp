#ifndef SPRITE_KEYFRAME_H
#define SPRITE_KEYFRAME_H

#include <Core/AnimationSys/IAnimation.hpp>
#include <Core/AnimationSys/Keyframe.hpp>
#include <Framework/Components/Sprite.hpp>

#include <vector>
#include <string>

namespace Essentia
{
	class SpriteKeyframe
	{
		public:
			std::string imageName;
			float duration = 0.5f;

			SpriteKeyframe(std::string imageReference, float duration)
				: imageName(std::move(imageReference)), duration(duration) {}

			SpriteKeyframe(std::string imageReference)
				: imageName(std::move(imageReference)) {}
	};
}

#endif // !SPRITE_KEYFRAME_H