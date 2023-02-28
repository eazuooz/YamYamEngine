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
		Scene(eSceneType type);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		eSceneType GetSceneType() { return mType; }
		Layer* GetLayer(eLayerType layer) { return &mLayers[(UINT)layer]; }
		void AddGameObject(GameObject* gameObject, eLayerType layerIndex);
		std::vector<GameObject*> GetDontDestroyGameObjects();

	private:
		eSceneType mType;
		Layer mLayers[(UINT)eLayerType::Max];
	};
}
