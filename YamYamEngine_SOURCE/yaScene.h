#pragma once
#include "yaEntity.h"
#include "yaLayer.h"

#define MAX_LAYER 32

namespace ya
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		std::vector<Layer> mLayers;
		//Layer mLayers[MAX_LAYER];
	};
}
