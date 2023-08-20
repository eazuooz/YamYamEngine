#include "yaAnimation.h"
#include "yaTime.h"
#include "yaRenderer.h"
#include "yaConstantBuffer.h"

namespace ya
{

	Animation::Animation()
		: mAtlas{}
		, mSpriteSheet{}
		, mAnimator(nullptr)
		, mIndex(0)
		, mTime(0.0f)
		, mbComplete(false)
	{
	}

	void Animation::Update()
	{
		if (mbComplete)
			return;

		// �ð� üũ
		mTime += Time::DeltaTime();

		// ���� �ð��� �ش� ������ �����ð��� �Ѿ�� �������������� �Ѿ
		if (mSpriteSheet[mIndex].duration < mTime)
		{
			mTime = 0.f;
			++mIndex;

			// �ִ� �����ӿ� �����ϸ� Finish ���·� ��ȯ
			if (mSpriteSheet.size() <= mIndex)
			{
				mIndex = static_cast<int>(mSpriteSheet.size() - 1);
				mbComplete = true;
			}
		}
	}
	void Animation::FixedUpdate()
	{
		__noop;
	}
	void Animation::Render()
	{
		__noop;
	}
	void Animation::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas
		, Vector2 leftTop, Vector2 size, Vector2 offset
		, float spriteLength, UINT column, float duration)
	{
		SetName(name);
		mAtlas = atlas;

		const UINT width = atlas->GetWidth();
		const UINT height = atlas->GetHeight();

		for (UINT i = 0; i < column; ++i)
		{
			Sprite frm = {};
			frm.leftTop = Vector2
			(
				(leftTop.x + spriteLength * static_cast<float>(i) / width
				, leftTop.y / height)
			);

			frm.size = Vector2(size.x / width, size.y / height);
			frm.offset = offset;
			frm.duration = duration;
			frm.atlasSize = Vector2(200.0f / width, 200.0f / height);

			mSpriteSheet.push_back(frm);
		}
	}

	void Animation::Binds() const
	{
		mAtlas->BindShaderResource(graphics::eShaderStage::PS, 12);

		graphics::ConstantBuffer* cb = renderer::constantBuffers[CAST_UINT(graphics::eCBType::Animator)];
		renderer::AnimatorCB info = {};
		info.type = CAST_UINT(enums::eAnimatorType::SecondDimension);
		info.leftTop = mSpriteSheet[mIndex].leftTop;
		info.offset = mSpriteSheet[mIndex].offset;
		info.size = mSpriteSheet[mIndex].size;
		info.atlasSize = mSpriteSheet[mIndex].atlasSize;

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::PS);
		//�ִϸ��̼� ������� ����
	}

	void Animation::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;
	}

	void Animation::Clear()
	{
		graphics::Texture::ClearShaderResourceView(12);
		graphics::ConstantBuffer* cb = renderer::constantBuffers[CAST_UINT(graphics::eCBType::Animator)];
		renderer::AnimatorCB info = {};
		info.type = CAST_UINT(enums::eAnimatorType::None);

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::PS);
	}

	bool Animation::IsComplete() const 
	{
		return mbComplete;
	}

}


