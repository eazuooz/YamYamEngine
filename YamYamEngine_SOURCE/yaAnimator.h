#pragma once
#include "yaComponent.h"
#include "yaTexture.h"
#include "yaAnimation.h"

namespace ya
{
	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}

			void operator()() const
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
		~Animator() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		bool CreateAnimation(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, Vector2 leftTop, Vector2 size, Vector2 offset
			, float columnLegth, UINT spriteLength, float duration);
		bool CreateAnimations(std::vector<FbxLoader::AnimationClip*> clips);

		Animation* Find(const std::wstring& name);
		void Play(const std::wstring& name, bool loop);
		void Bind(int boneCount = 0);
		void Clear();

		Events* FindEvents(const std::wstring key);
		std::function<void()>& GetStartEvent(const std::wstring key);
		std::function<void()>& GetCompleteEvent(const std::wstring key);
		std::function<void()>& GetEndEvent(const std::wstring key);

	
	private:
		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		Animation* mActiveAnimation;
		bool mbLoop;
	};
}
