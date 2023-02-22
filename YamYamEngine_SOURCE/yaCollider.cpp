#include "yaCollider.h"
#include "yaScript.h"
#include "yaGameObject.h"

namespace ya
{
	


	Collider::Collider()
		: Component(eComponentType::Collider)
		, mTransform(nullptr)
		, mCollisionCount(0)
		, mbIsTrigger(false)
	{
	}

	Collider::~Collider()
	{
	}

	void Collider::Initialize()
	{
	}

	void Collider::Update()
	{
	}

	void Collider::FixedUpdate()
	{
	}

	void Collider::Render()
	{
	}

	void Collider::OnCollisionEnter(Collider* collider)
	{
		mCollisionCount++;
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(collider);
		}
	}

	void Collider::OnCollisionStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(collider);
		}
	}

	void Collider::OnCollisionExit(Collider* collider)
	{
		mCollisionCount--;
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(collider);
		}
	}

	void Collider::OnTriigerEnter(Collider* collider)
	{
		mCollisionCount++;
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriigerEnter(collider);
		}
	}

	void Collider::OnTriigerStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriigerStay(collider);
		}
	}

	void Collider::OnTriigerExit(Collider* collider)
	{
		mCollisionCount--;
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriigerExit(collider);
		}
	}

}
