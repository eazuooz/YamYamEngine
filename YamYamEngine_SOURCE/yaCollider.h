#pragma once

#include "yaComponent.h"
#include "yaTransform.h"


namespace ya
{
	class Collider : public Component
	{
	public:
		Collider();
		~Collider();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider* collider);
		virtual void OnCollisionStay(Collider* collider);
		virtual void OnCollisionExit(Collider* collider);
		
		virtual void OnTriigerEnter(Collider* collider);
		virtual void OnTriigerStay(Collider* collider);
		virtual void OnTriigerExit(Collider* collider);

		void SetType(eColliderType type) { mType = type; }
		bool IsTrigger() { return mbIsTrigger; }
		Vector3 GetColliderPos() { return mColliderPosition; }

	protected:
		eColliderType mType;
		Transform* mTransform;
		bool mbIsTrigger;
		UINT mCollisionCount;
		Vector3 mColliderPosition;
	};
}
