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
	};
}
