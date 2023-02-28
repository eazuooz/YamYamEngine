#include "yaAnimator.h"


namespace ya
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mbLoop(false)
		, mActiveAnimation(nullptr)
	{
	}

	Animator::~Animator()
	{
		for (auto animation : mAnimations)
		{
			delete animation.second;
			animation.second = nullptr;
		}
	}

	void Animator::Initialize()
	{

	}

	void Animator::Update()
	{
	}

	void Animator::FixedUpdate()
	{
		if (mActiveAnimation == nullptr)
			return;

		if (mActiveAnimation->IsComplete() && mbLoop)
		{
			mActiveAnimation->Reset();
		}

		mActiveAnimation->FixedUpdate();
	}

	void Animator::Render()
	{
	}

	bool Animator::CreateAnimation(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, Vector2 offset, float spriteLegth, UINT column, float duration)
	{
		if (atlas.get() == nullptr)
			return false;

		Animation* animation = Find(name);
		if (animation != nullptr)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, leftTop, size, offset, spriteLegth, column, duration );

		mAnimations.insert(std::make_pair(name, animation));
	}

	Animation* Animator::Find(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter = mAnimations.find(name);
		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}

	void Animator::Play(const std::wstring& name, bool loop)
	{
		Animation* animation = Find(name);
		if (animation)
		{
			mActiveAnimation = animation;
		}

		mbLoop = loop;
		mActiveAnimation->Reset();
	}
	void Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Binds();
	}
}