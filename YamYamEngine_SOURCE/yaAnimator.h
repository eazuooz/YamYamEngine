#pragma once
#include "yaComponent.h"
#include "yaTexture.h"
#include "yaAnimation.h"

namespace ya
{
	using namespace ya::graphics;
	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}

			void operator()()
			{
				if (mEvent)
					mEvent();
			}


			std::function<void()> mEvent;
		};
		struct Events
		{
			Event mStartEvent;
			Event mCompleteEvent;
			Event mEndEvent;
		};

		Animator();
		~Animator();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void CreateAnimation(const std::wstring& name, std::shared_ptr<Texture> atlas
							, Vector2 leftTop, Vector2 slice, float duration, int maxFrm, float fps);
		Animation* Find(const std::wstring& name);
		void Play(const std::wstring& name, bool loop);

	private:
		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		Animation* mActiveAnimation;

		bool mbLoop;
	};
}
