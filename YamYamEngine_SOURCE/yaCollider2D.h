#pragma once
#include "yaCollider.h"

namespace ya
{
	class Collider2D : public Collider
	{
	public:
		Collider2D();
		~Collider2D();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		virtual void OnTriigerEnter(Collider* collider) override;
		virtual void OnTriigerStay(Collider* collider) override;
		virtual void OnTriigerExit(Collider* collider) override;

	//	void SetSize(Vector2 size) { mSize = size; }
	//	void SetCenter(Vector2 center) { mCenter = center; }
	//	Vector2 GetSize() { return mSize; }
	//	Vector2 GetCenter() { return mCenter; }
	//
	//private:
	//	Vector2 mSize;
	//	Vector2 mCenter;
		
	};
}
