#include "yaGameObject.h"


namespace ya
{
	GameObject::GameObject()
		: mState(eState::Active)
	{
		mComponents.resize(COMPONENTTYPE::END);
	}

	GameObject::~GameObject()
	{

	}

	void GameObject::AddComponent(Component* component)
	{
		int myOrder = component->GetUpdateOrder();
		mComponents[myOrder] = component;
		mComponents[myOrder]->mOwner = this;
	}
	void GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Initialize();
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
	}
	void GameObject::FixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
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
}
