#include "yaAnimation.h"
#include "yaTime.h"
#include "yaRenderer.h"
#include "yaGameObject.h"
#include "yaConstantBuffer.h"
#include "yaTransformBoneShader.h"
#include "yaResources.h"
#include "yaSkinnedMeshRenderer.h"
#include "yaAnimator.h"

namespace ya
{
	Animation::Animation()
		: mAtlas{}
		, mSpriteSheet{}
		, mAnimator(nullptr)
		, mIndex(0)
		, mTime(0.0f)
		, mbComplete(false)
		, mType(enums::eAnimationType::None)
		, mClip{}
		, mFrameIndex(0)
		, mNextFrameIndex(0)
		, mFrameRatio(0.0f)
	{
	}

	Animation::~Animation()
	{

	}

	void Animation::Update()
	{
		if (mType == eAnimationType::SecondDimension)
			UpdateSprite();
		else if (mType == eAnimationType::ThridDimension)
			UpdateBone();
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

	void Animation::Create(const std::wstring& name, FbxLoader::AnimationClip* clip)
	{
		mClip.name = name;
		mClip.startTime = clip->startTime.GetSecondDouble();
		mClip.endTime = clip->endTime.GetSecondDouble();
		mClip.timeLength = mClip.endTime - mClip.startTime;

		mClip.mode = (UINT)clip->mode;
		mClip.startFrame = (int)clip->startTime.GetFrameCount(clip->mode);
		mClip.endFrame = (int)clip->endTime.GetFrameCount(clip->mode);
		mClip.frameLength = mClip.endFrame - mClip.startFrame;
	}

	void Animation::Bind(int boneCount)
	{
		if (mType == eAnimationType::SecondDimension)
			BindSprite();
		else if (mType == eAnimationType::ThridDimension)
			BindClip(boneCount);
	}

	void Animation::BindSprite()
	{
		graphics::ConstantBuffer* cb = renderer::constantBuffers[CAST_UINT(graphics::eCBType::Animator)];
		renderer::AnimatorCB info = {};
		info.type = CAST_UINT(enums::eAnimationType::SecondDimension);
		info.leftTop = mSpriteSheet[mIndex].leftTop;
		info.offset = mSpriteSheet[mIndex].offset;
		info.size = mSpriteSheet[mIndex].size;
		info.atlasSize = mSpriteSheet[mIndex].atlasSize;

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::VS);
		cb->Bind(graphics::eShaderStage::PS);
		//cb->Bind(graphics::eShaderStage::CS);

		mAtlas->BindShaderResource(graphics::eShaderStage::PS, 12);
	}

	void Animation::BindClip(int boneCount)
	{
		graphics::ConstantBuffer* cb = renderer::constantBuffers[CAST_UINT(graphics::eCBType::Animator)];
		renderer::AnimatorCB info = {};
		info.type = CAST_UINT(enums::eAnimationType::ThridDimension);
		info.boneCount = boneCount;
		info.frameIdx = mFrameIndex;
		info.nextFrameIdx = mNextFrameIndex;
		info.frameRatio = mFrameRatio;

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::VS);
		cb->Bind(graphics::eShaderStage::PS);
		cb->Bind(graphics::eShaderStage::CS);

		std::shared_ptr<graphics::TransformBoneShader> skeletonShader
			= Resources::Find<graphics::TransformBoneShader>(L"TransformBoneCS");

		graphics::StructedBuffer* globalBuffer = mAnimator->GetGlobalMatrices();
		skeletonShader->SetGlobalMatrices(globalBuffer);

		GameObject* obj = mAnimator->GetOwner();
		SkinnedMeshRenderer* mr = obj->GetComponent<SkinnedMeshRenderer>();
		std::shared_ptr<Mesh> mesh = mr->GetMesh();
		std::vector<MeshData*> meshDatas = mesh->GetMeshDatas();

		skeletonShader->SetOffsetMatrices(meshDatas[0]->offsetMatrices);
		skeletonShader->SetRootMatrices(meshDatas[0]->rootMatrices);
		skeletonShader->SetAnimatorCB(info);
		skeletonShader->OnExcute();

		globalBuffer->BindSRV(eShaderStage::VS, 30);
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
		info.type = CAST_UINT(enums::eAnimationType::None);

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::VS);
		cb->Bind(graphics::eShaderStage::PS);
		cb->Bind(graphics::eShaderStage::CS);

		graphics::StructedBuffer* globalBuffer = mAnimator->GetGlobalMatrices();
		globalBuffer->Clear();
	}

	bool Animation::IsComplete() const 
	{
		return mbComplete;
	}
	
	void Animation::UpdateSprite()
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
				mIndex = static_cast<int>(mSpriteSheet.size() - 1);
				mbComplete = true;
			}
		}
	}

	void Animation::UpdateBone()
	{
		if (mbComplete)
			return;

		mTime += Time::DeltaTime();

		if (mTime >=  mClip.timeLength) 
			mTime = 0.0f;

		UpdateFrameIndex();
	}

	void Animation::UpdateFrameIndex()
	{
		// FBX 파일마다 정해져 있음
		const int frameCount = 30;

		double currentTime = mClip.startTime + mTime;
		double frameIndex = currentTime * (double)frameCount;
		mFrameIndex = (int)frameIndex;

		// 다음 프레임 인덱스
		if (mFrameIndex >= mClip.frameLength - 1)
			mNextFrameIndex = mFrameIndex;	
		else
			mNextFrameIndex = mFrameIndex + 1;

		mFrameRatio = (float)(frameIndex - (double)mFrameIndex);
	}
}


