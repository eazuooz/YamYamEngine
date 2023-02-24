#include "yaAnimation.h"
#include "yaTime.h"


namespace ya
{
	Animation::Animation()
	{
	}
	Animation::~Animation()
	{
	}
	void Animation::Update()
	{
	}
	void Animation::FixedUpdate()
	{
		if (mbComplete)
			return;

		// 시간 체크
		mTime += Time::DeltaTime();

		// 누적 시간이 해당 프레임 유지시간을 넘어서면 다음프레임으로 넘어감
		if (mSpriteSheet[mIndex].duration < mTime)
		{
			mTime = 0.f;
			++mIndex;

			// 최대 프레임에 도달하면 Finish 상태로 전환
			if (mSpriteSheet.size() <= mIndex)
			{
				mIndex = mSpriteSheet.size() - 1;
				mbComplete = true;
			}
		}
	}
	void Animation::Render()
	{
	}
	void Animation::Create(std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, Vector2 offset, float columnLegth, UINT spriteLength, float duration)
	{
		// Animation Name
		SetName(name);
		mAtlas = atlas;
		float width = (float)atlas->GetWidth();
		float height = (float)atlas->GetHeight();

		// Frame Info
		for (int i = 0; i < spriteLength; ++i)
		{
			Sprite frm = {};
			frm.leftTop = Vector2((leftTop.x + columnLegth * (float)i) / width, leftTop.y / height);
			frm.size = Vector2(size.x / width, size.y / height) ;
			frm.offset = offset;
			frm.duration = duration;

			mSpriteSheet.push_back(frm);
		}
	}

	void Animation::Binds()
	{
		mAtlas->BindShader(eShaderStage::PS, 12);

		//애니메이션 상수버퍼 제작
	}

	void Animation::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;
	}
}
