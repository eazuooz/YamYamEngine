#pragma once
#include "yaEnums.h"
#include "yaEntity.h"
#include "yaLayer.h"

namespace ya
{
	using namespace ya::enums;

	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		Layer* GetLayer(UINT index) { return& mLayers[index]; }
		void AddGameObject(GameObject* gameObject, eLayer layerIndex);

	private:
		Layer mLayers[(UINT)eLayer::Max];
	};
}
