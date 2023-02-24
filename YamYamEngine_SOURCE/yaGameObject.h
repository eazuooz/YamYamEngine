#pragma once
#include "yaEntity.h"
#include "yaComponent.h"
#include "yaScript.h"

namespace ya
{
	class GameObject : public Entity
	{
	public:
		enum class eState
		{
			Active,
			Paused,
			Dead,
		};

		GameObject();
		virtual ~GameObject();

		template <typename T>
		T* AddComponent()
		{
			T* component = new T();
			int myOrder = component->GetUpdateOrder();
			if (eComponentType::Script == (eComponentType)myOrder)
			{
				mScripts.push_back(dynamic_cast<Script*>(component));
				component->mOwner = this;
			}
			else
			{
				mComponents[myOrder] = component;
				mComponents[myOrder]->mOwner = this;
			}

			return component;
		}
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
		std::vector<Script*>& GetScripts() { return mScripts; }

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void SetActive(bool value);
		eState GetActive() { return mState; }
		bool IsDead()
		{
			if (mState == eState::Dead)
				return true;

			return false;
		}
		void Death();

	protected:
		std::vector<Component*> mComponents;

	private:
		eState mState;
		std::vector<Script*> mScripts;
	};
}
