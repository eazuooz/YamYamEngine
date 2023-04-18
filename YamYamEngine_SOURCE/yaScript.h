#pragma once
#include "yaComponent.h"
#include "yaCollider.h"

namespace ya
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		
		virtual void OnCollisionEnter(Collider* collider);
		virtual void OnCollisionStay(Collider* collider);
		virtual void OnCollisionExit(Collider* collider);

		virtual void OnTriigerEnter(Collider* collider);
		virtual void OnTriigerStay(Collider* collider);
		virtual void OnTriigerExit(Collider* collider);

	private:

	};
}
