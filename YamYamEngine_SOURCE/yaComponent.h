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

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void Render() = 0;

		GameObject* GetOwner() { return mOwner; }
		UINT GetUpdateOrder() { return (UINT)mType; }

	private:
		const eComponentType mType;
		GameObject* mOwner;
	};
}
