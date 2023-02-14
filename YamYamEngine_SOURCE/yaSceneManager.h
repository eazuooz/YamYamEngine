#pragma once
#include "yaScene.h"

namespace ya
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Destroy();
		static void Release();

		static Scene* GetActiveScene() { return mActiveScene; }

	private:
		static Scene* mActiveScene;
	};
}
