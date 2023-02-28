#pragma once
#include "yaEntity.h"
#include "yaTexture.h"


namespace ya
{
	using namespace ya::math;
	using namespace ya::graphics;

	class Animator;
	class Animation : public Entity
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;	// ���� ��� �ȼ���ǥ
			Vector2 size;		// ���� ������� ���� �߶� ���� ���� �ȼ� ����
			Vector2 offset;
			Vector2 atlasSize;
			float duration;			// �ش� ������ �����ð�

			Sprite()
				: leftTop(0.0f, 0.0f)
				, size(0.0f, 0.0f)
				, offset(0.0f, 0.0f)
				, atlasSize(0.0f, 0.0f)
				, duration(0.0f)
			{

			}
		};

		Animation();
		~Animation();

		void Update();
		void FixedUpdate();
		void Render();


		void Create(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, Vector2 offset
			, float columnLegth, UINT spriteLength, float duration);
		void Binds();

		void Reset();
		bool IsComplete() { return mbComplete; }

	private:
		std::shared_ptr<Texture> mAtlas;
		Animator* mAnimator;
		std::vector<Sprite> mSpriteSheet;
		int mIndex;
		float mTime;
		bool mbComplete;
	};
}
