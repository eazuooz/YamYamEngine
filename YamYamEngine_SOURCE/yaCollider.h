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
		virtual void LateUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider* collider);
		virtual void OnCollisionStay(Collider* collider);
		virtual void OnCollisionExit(Collider* collider);
		
		virtual void OnTriigerEnter(Collider* collider);
		virtual void OnTriigerStay(Collider* collider);
		virtual void OnTriigerExit(Collider* collider);

		eColliderType GetType() { return mType; }
		void SetType(eColliderType type) { mType = type; }
		bool IsTrigger() { return mbIsTrigger; }
		Vector3 GetColliderPos() { return mColliderPosition; }
		void SetSize(Vector2 size) { mSize = size; }
		void SetCenter(Vector2 center) { mCenter = center; }
		Vector2 GetSize() { return mSize; }
		Vector2 GetCenter() { return mCenter; }

	protected:
		eColliderType mType;
		Transform* mTransform;
		bool mbIsTrigger;
		UINT mCollisionCount;
		Vector3 mColliderPosition;

		Vector2 mSize;
		Vector2 mCenter;
	};
}
