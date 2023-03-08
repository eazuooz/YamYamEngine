#include "yaAnimation.h"
#include "yaTime.h"
#include "yaRenderer.h"
#include "yaConstantBuffer.h"

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
				mIndex = (int)mSpriteSheet.size() - 1;
				mbComplete = true;
			}
		}
	}
	void Animation::FixedUpdate()
	{

	}
	void Animation::Render()
	{
	}
	void Animation::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, Vector2 offset, float spriteLength, UINT column, float duration)
	{
		// Animation Name
		SetName(name);
		mAtlas = atlas;
		float width = (float)atlas->GetWidth();
		float height = (float)atlas->GetHeight();

		// Frame Info
		for (int i = 0; i < column; ++i)
		{
			Sprite frm = {};
			frm.leftTop = Vector2( (leftTop.x + spriteLength * (float)i) / width, leftTop.y / height );
			frm.size = Vector2(size.x / width, size.y / height) ;
			frm.offset = offset;
			frm.duration = duration;
			//frm.atlasSize = Vector2(width, height);
			frm.atlasSize = Vector2(200.0f / width, 200.0f / height);

			mSpriteSheet.push_back(frm);
		}
	}

	void Animation::Binds()
	{
		mAtlas->BindShader(eShaderStage::PS, 12);

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Animator];
		renderer::AnimatorCB info = {};
		info.type = (UINT)eAnimatorType::SecondDimension;
		info.leftTop = mSpriteSheet[mIndex].leftTop;
		info.offset = mSpriteSheet[mIndex].offset;
		info.size = mSpriteSheet[mIndex].size;
		info.atlasSize = mSpriteSheet[mIndex].atlasSize;
		
		cb->Bind(&info);
		cb->SetPipline(eShaderStage::PS);
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
		Texture::Clear(12);
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Animator];
		renderer::AnimatorCB info = {};
		info.type = (UINT)eAnimatorType::None;

		cb->Bind(&info);
		cb->SetPipline(eShaderStage::PS);
	}


}
