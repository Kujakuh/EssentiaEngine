#include <Core/AnimationSys/SpriteAnimation.hpp>

namespace Essentia
{
	void SpriteAnimation::Update(float deltaTime)
	{
		if (m_Frames.empty()) return;

		m_CurrentTime += deltaTime * m_Speed;

		while (m_CurrentTime >= m_Frames[m_CurrentFrameIndex].duration) {
			const auto& currentFrame = m_Frames[m_CurrentFrameIndex];

			m_CurrentTime -= currentFrame.duration;
			m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_Frames.size();


			if (HasFileExtension(currentFrame.imageName))
				m_Sprite.setTexture(currentFrame.imageName);
			else
				m_Sprite.useRegionFromAtlas(currentFrame.imageName);
		}
	}

	void SpriteAnimation::AddFrame(const SpriteKeyframe& frame)
	{
		m_Frames.push_back(frame);
		m_Duration += frame.duration;
	}

	bool SpriteAnimation::HasFileExtension(const std::string& filename)
	{
		size_t dotPos = filename.find_last_of('.');
		return (dotPos != std::string::npos &&
			dotPos != 0 &&
			dotPos < filename.length() - 1);
	}

	void SpriteAnimation::Reset()
	{
		m_CurrentFrameIndex = 0;
		m_CurrentTime = 0.0f;

		if (!m_Frames.empty())
		{
			const auto& firstFrame = m_Frames[m_CurrentFrameIndex];
			if (HasFileExtension(firstFrame.imageName))
				m_Sprite.setTexture(firstFrame.imageName);
			else
				m_Sprite.useRegionFromAtlas(firstFrame.imageName);
		}
	}
}
