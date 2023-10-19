#include "yaAnimator.h"


namespace ya
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mbLoop(false)
		, mActiveAnimation(nullptr)
	{
	}

	Animator::~Animator()
	{
		for (std::pair<std::wstring, Animation*> animation : mAnimations)
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
		if (mActiveAnimation == nullptr)
			return;

		if (mActiveAnimation->IsComplete() && mbLoop)
		{
			Events* events
				= FindEvents(mActiveAnimation->GetName());
			if (events)
				events->mCompleteEvent();

			mActiveAnimation->Reset();
		}

		mActiveAnimation->Update();
	}

	void Animator::LateUpdate()
	{

	}

	void Animator::Render()
	{
	}

	bool Animator::CreateAnimation(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas
		, Vector2 leftTop, Vector2 size, Vector2 offset
		, float spriteLegth, UINT column, float duration)
	{
		if (atlas.get() == nullptr)
			return false;

		Animation* animation = Find(name);
		if (animation != nullptr)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, leftTop, size, offset, spriteLegth, column, duration );

		mAnimations.insert(std::make_pair(name, animation));

		return true;
	}

	bool Animator::CreateAnimations(std::vector<FbxLoader::AnimationClip*> clips)
	{
		for (FbxLoader::AnimationClip* clip : clips)
		{
			Animation* animation = Find(clip->name);
			if (animation != nullptr)
				return false;

			animation = new Animation();
			animation->SetType(enums::eAnimationType::ThridDimension);
			animation->SetName(clip->name);
			animation->Create(clip->name, clip);

			mAnimations.insert(std::make_pair(clip->name, animation));
		}

		return true;
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
		Animator::Events* events = FindEvents(name);
		if (events != nullptr)
			events->mStartEvent();

		Animation* prevAnimation = mActiveAnimation;
		mActiveAnimation = Find(name);
		mActiveAnimation->Reset();
		mbLoop = loop;

		if (prevAnimation != mActiveAnimation)
		{
			if (events != nullptr)
				events->mEndEvent();
		}
	}

	void Animator::Bind(int boneCount)
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Bind(boneCount);
	}
	void Animator::Clear()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Clear();
	}
	Animator::Events* Animator::FindEvents(const std::wstring key)
	{
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(key);
		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mStartEvent.mEvent;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mCompleteEvent.mEvent;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mEndEvent.mEvent;
	}
}