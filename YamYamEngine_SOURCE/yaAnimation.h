#pragma once
#include "yaEntity.h"
#include "yaTexture.h"


namespace ya
{
	class Animator;
	class Animation : public Entity
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;	// 좌측 상단 픽셀좌표
			Vector2 size;		// 좌측 상단으로 부터 잘라낼 가로 세로 픽셀 길이
			Vector2 offset;
			Vector2 atlasSize;
			float duration;			// 해당 프레임 유지시간

			Sprite()
				: leftTop()
				, size()
				, offset()
				, atlasSize()
				, duration(0.0f)
			{


			}
		};

		Animation();
		~Animation() override = default;

		void Update();
		void FixedUpdate();
		void Render();


		void Create(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, Vector2 leftTop, Vector2 size, Vector2 offset
			, float columnLegth, UINT spriteLength, float duration);
		void Binds() const;
		void Reset();
		void Clear();
		bool IsComplete() const;

	private:
		std::shared_ptr<graphics::Texture> mAtlas;
		std::vector<Sprite> mSpriteSheet;
		Animator* mAnimator;
		int mIndex;
		float mTime;
		bool mbComplete;
	};
}
