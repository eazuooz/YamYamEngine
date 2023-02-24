#include "yaAnimator.h"


namespace ya
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mbRepeat(false)
		, mActiveAnimation(nullptr)
	{
	}

	Animator::~Animator()
	{
	}

	void Animator::Initialize()
	{

	}

	void Animator::Update()
	{
	}

	void Animator::FixedUpdate()
	{
	}

	void Animator::Render()
	{
	}

	void Animator::CreateAnimation(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 slice, float duration, int maxFrm, float fps)
	{
	}

	Animation* Animator::Find(const std::wstring& name)
	{
		return nullptr;
	}

	void Animator::Play(const std::wstring& name, bool repeat)
	{
	}
}