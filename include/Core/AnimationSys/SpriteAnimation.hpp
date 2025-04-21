#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include <Core/AnimationSys/IAnimation.hpp>
#include <Core/AnimationSys/SpriteKeyframe.hpp>
#include <Framework/Components/Sprite.hpp>

#include <iostream>

namespace Essentia
{

	class SpriteAnimation : public IAnimation
	{
		public:
			SpriteAnimation(Sprite& sprite, float speed = 1.0f)
				: m_Sprite(sprite), m_CurrentFrameIndex(0)
			{m_Speed = speed;}

			SpriteAnimation(const SpriteAnimation&) = default;
			SpriteAnimation() = default;

			void AddFrame(const SpriteKeyframe& frame);
			void Update(float deltaTime) override;
			void Reset();

		private:
			bool HasFileExtension(const std::string& filename);

			std::vector<SpriteKeyframe> m_Frames;
			Sprite& m_Sprite;
			size_t m_CurrentFrameIndex = 0;
	};
}

#endif // !SPRITE_ANIM_H