#pragma once
#include "yaEnums.h"
#include "yaEntity.h"
#include "yaMath.h"

namespace ya
{
	using namespace enums;
	using namespace math;

	class GameObject;
	class Component : public Entity
	{
	public:
		friend GameObject;

		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		GameObject* GetOwner() { return mOwner; }
		UINT GetUpdateOrder() { return (UINT)mType; }

	private:
		const eComponentType mType;
		GameObject* mOwner;
	};
}
