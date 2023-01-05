#pragma once
#include "yaEntity.h"
#include "yaComponent.h"
#include "yaScript.h"

namespace ya
{
	class GameObject : public Entity
	{
	public:
		enum eState
		{
			Active,
			Paused,
			Dead,
		};

		GameObject();
		virtual ~GameObject();

		void AddComponent(Component* component);
		template <typename T>
		T* GetComponent()
		{
			T* component;
			for (auto c : mComponents)
			{
				component = dynamic_cast<T*>(c);

				if (component != nullptr)
					return component;
			}

			return nullptr;
		}

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		eState mState;
		std::vector<Component*> mComponents;
		std::vector<Script*> mScripts;
	};
}
