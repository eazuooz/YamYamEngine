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
		virtual void Destroy();

		Layer* GetLayer(eLayerType layer) { return& mLayers[(UINT)layer]; }
		void AddGameObject(GameObject* gameObject, eLayerType layerIndex);

	private:
		Layer mLayers[(UINT)eLayerType::Max];
	};
}
