#include "yaGameObject.h"


namespace ya
{
	GameObject::GameObject()
		: mState(eState::Active)
		, mType(eLayerType::None)
		, mbDontDestroy(false)
	{
		mComponents.resize((UINT)eComponentType::End);
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			delete comp;
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			delete script;
		}
	}

	void GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Initialize();
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->Initialize();
		}
	}
	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Update();
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->Update();
		}
	}
	void GameObject::FixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->FixedUpdate();
		}
	}
	void GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Render();
		}
	}

	void GameObject::SetActive(bool value)
	{
		if (value == true)
			mState = eState::Active;
		else
			mState = eState::Paused;
	}

	void GameObject::Death()
	{
		mState = eState::Dead;
	}
}
